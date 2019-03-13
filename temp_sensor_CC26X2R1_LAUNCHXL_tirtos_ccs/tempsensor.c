/******************************************************************************

 @file tempsensor.c

 @brief Temperature Sensor example application

 Group: CMCU, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2017-2019, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/
#include <openthread/config.h>
#include <openthread-core-config.h>

/* Standard Library Header files */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* POSIX Header files */
#include <time.h>
#include <unistd.h>


/* OpenThread public API Header files */
#include <openthread/coap.h>
#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/platform/logging.h>
#include <openthread/platform/uart.h>
#include <openthread/thread.h>
#include <openthread/icmp6.h>


/* TIRTOS specific header files */
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/BIOS.h>

/* driverlib specific header */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)
#include <ti/drivers/GPIO.h>

/* POSIX Header files */
#include <sched.h>
#include <pthread.h>

/* OpenThread Internal/Example Header files */
#include "otsupport/otrtosapi.h"
#include "otsupport/otinstance.h"

/* Board Header files */
#include "Board.h"

#include "tempsensor.h"
#include "utils/code_utils.h"
#include "disp_utils.h"
#include "keys_utils.h"
#include "otstack.h"

/* Private configuration Header files */
#include "task_config.h"
#include "tiop_config.h"

#if (OPENTHREAD_ENABLE_APPLICATION_COAP == 0)
#error "OPENTHREAD_ENABLE_APPLICATION_COAP needs to be defined and set to 1"
#endif

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/* read attribute */
#define ATTR_READ     0x01
/* write attribute */
#define ATTR_WRITE    0x02
/* report attribute */
#define ATTR_REPORT   0x04

/* Reporting interval in milliseconds */
#ifndef TIOP_TEMPSENSOR_REPORTING_INTERVAL
#define TIOP_TEMPSENSOR_REPORTING_INTERVAL 10000
#endif

/* Address to report temperature */
#ifndef TIOP_TEMPSENSOR_REPORTING_ADDRESS
#define TIOP_TEMPSENSOR_REPORTING_ADDRESS  "ff02::1"
#define TIOP_OWN_REPORTING_ADDRESS "64:ff9b::8e5d:886d"
#endif

#define DEFAULT_COAP_HEADER_TOKEN_LEN      2

/* coap attribute descriptor */
typedef struct
{
    const char*    uriPath; /* attribute URI */
    uint16_t       type;    /* type of resource: read only or read write */
    uint8_t*       pValue;  /* pointer to value of attribute state */

} attrDesc_t;

/******************************************************************************
 Local variables
 *****************************************************************************/

/* Timer ID */
static timer_t reportTimerID;

/* Global IPv6 address configured via SLAAC update */
static otIp6Address globalAddress;

/* IPv6 address to send the reporting temperature to */
static otIp6Address thermostatAddress;

/* Port to report the temperature to */
static uint16_t peerPort = OT_DEFAULT_COAP_PORT;

/* TI-RTOS events structure for passing state to the processing loop */
static Event_Struct tempSensorEvents;

/* OpenThread Stack thread call stack */
static char stack[TASK_CONFIG_TEMPSENSOR_TASK_STACK_SIZE];

/* coap resource for the application */
static otCoapResource coapResource;

/* coap attribute state of the application */
static uint8_t attrTemperature[11] = "70";
static int temperatureValue = 70;

/* coap attribute descriptor for the application */
const attrDesc_t coapAttr = {
    TEMPSENSOR_TEMP_URI,
    (ATTR_READ|ATTR_REPORT),
    attrTemperature,
};

/* ping handler statics */
static otIcmp6Handler cli_icmpHandler;

/* Holds the server setup state: 1 indicates CoAP server has been setup */
static bool serverSetup;

/******************************************************************************
 Function Prototype
 *****************************************************************************/

/* Temperature Sensor processing thread. */
static void *TempSensor_task(void *arg0);
/* Timeout callback for reporting. */
static void reportingTimeoutCB(union sigval val);

/******************************************************************************
 Local Functions
 *****************************************************************************/

/**
 * @brief Configure the timer.
 *
 * @return None
 */
static void configureReportingTimer(void)
{
    struct sigevent event =
    {
        .sigev_notify_function = reportingTimeoutCB,
        .sigev_notify          = SIGEV_SIGNAL,
    };

    timer_create(CLOCK_MONOTONIC, &event, &reportTimerID);
}

/**
* Handler for ICMPv6 messages.
*/
void cli_icmp6RxCallback(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo,
                        const otIcmp6Header *aIcmpHeader)
{
   (void)aContext;
   (void)aMessage;
   (void)aMessageInfo;

   if (OT_ICMP6_TYPE_ECHO_REQUEST == aIcmpHeader->mType)
   {
       GPIO_toggle(Board_GPIO_GLED);
       sleep(1);
       GPIO_toggle(Board_GPIO_GLED);

   }
}

/**
 * @brief Starts the reporting timer.
 *
 * @param timeout Time in milliseconds.
 *
 * Should be called after the reporting timer has been created, and after the
 * global address has been registered.
 */
static void startReportingTimer(uint32_t timeout)
{
    struct itimerspec newTime  = {0};
    struct itimerspec zeroTime = {0};
    struct itimerspec currTime;

    newTime.it_value.tv_sec  = (timeout / 1000U);
    newTime.it_value.tv_nsec = ((timeout % 1000U) * 1000000U);

    /* Disarm timer if currently armed */
    timer_gettime(reportTimerID, &currTime);
    if ((currTime.it_value.tv_sec != 0) || (currTime.it_value.tv_nsec != 0))
    {
        timer_settime(reportTimerID, 0, &zeroTime, NULL);
    }

    /* Arm timer */
    timer_settime(reportTimerID, 0, &newTime, NULL);
}

/**
 * @brief Callback function registered with the Coap server.
 *        Processes the coap request from the clients.
 *
 * @param val Argument passed by the clock if set up.
 *
 * @return None
 */
static void reportingTimeoutCB(union sigval val)
{
    TempSensor_postEvt(TempSensor_evtReportTemp);

    (void) val;
}

/**
 * @brief Reports the temperature to another coap device.
 *
 * @return None
 */
static void tempSensorReport(void)
{
    otError error = OT_ERROR_NONE;
    otMessage *requestMessage = NULL;
    otMessageInfo messageInfo;
    otCoapHeader requestHeader;
    otInstance *instance = OtInstance_get();
    int32_t celsiusTemp;

    /* make sure there is a new temperature reading otherwise just report the previous temperature */
    if(AONBatMonNewTempMeasureReady())
    {
        /* Read the temperature in degrees C from the internal temp sensor */
        celsiusTemp = AONBatMonTemperatureGetDegC();

        /* convert temp to Fahrenheit */
        temperatureValue = (int)((celsiusTemp * 9) / 5) + 32;
        /* convert temperature to string attribute */
        snprintf((char*)attrTemperature, sizeof(attrTemperature), "%d",
                 temperatureValue);
    }

    /* print the reported value to the terminal */
    DISPUTILS_SERIALPRINTF(0, 0, "Reporting Temperature:");
    DISPUTILS_SERIALPRINTF(0, 0, (char*)attrTemperature);


    OtRtosApi_lock();
    otCoapHeaderInit(&requestHeader, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_POST);
    otCoapHeaderGenerateToken(&requestHeader, DEFAULT_COAP_HEADER_TOKEN_LEN);
    error = otCoapHeaderAppendUriPathOptions(&requestHeader,
                                             THERMOSTAT_TEMP_URI);
    OtRtosApi_unlock();
    otEXPECT(OT_ERROR_NONE == error);

    OtRtosApi_lock();
    otCoapHeaderSetPayloadMarker(&requestHeader);
    requestMessage = otCoapNewMessage(instance, &requestHeader);
    OtRtosApi_unlock();
    otEXPECT_ACTION(requestMessage != NULL, error = OT_ERROR_NO_BUFS);

    OtRtosApi_lock();
    error = otMessageAppend(requestMessage, attrTemperature,
                            strlen((const char*) attrTemperature));
    OtRtosApi_unlock();
    otEXPECT(OT_ERROR_NONE == error);

    memset(&messageInfo, 0, sizeof(messageInfo));
    messageInfo.mPeerAddr = thermostatAddress;
    messageInfo.mPeerPort = peerPort;
    messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;

    OtRtosApi_lock();
    error = otCoapSendRequest(instance, requestMessage, &messageInfo, NULL,
                              NULL);
    OtRtosApi_unlock();

    /* Restart the clock */
    startReportingTimer(TIOP_TEMPSENSOR_REPORTING_INTERVAL);
    exit:

    if(error != OT_ERROR_NONE && requestMessage != NULL)
    {
        OtRtosApi_lock();
        otMessageFree(requestMessage);
        OtRtosApi_unlock();
    }
}

static void sendMessage(void) {


    otError error = OT_ERROR_NONE;
    otMessage *requestMessage = NULL;
    otMessageInfo messageInfo;
    otCoapHeader requestHeader;
    otInstance *instance = OtInstance_get();

    /* print the reported value to the terminal */
    DISPUTILS_SERIALPRINTF(0, 0, "Attempting to send coap:");

    OtRtosApi_lock();
    otCoapHeaderInit(&requestHeader, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_POST);
    otCoapHeaderGenerateToken(&requestHeader, DEFAULT_COAP_HEADER_TOKEN_LEN);
    error = otCoapHeaderAppendUriPathOptions(&requestHeader,
                                             THERMOSTAT_TEMP_URI);
    OtRtosApi_unlock();
    otEXPECT(OT_ERROR_NONE == error);

    OtRtosApi_lock();
    otCoapHeaderSetPayloadMarker(&requestHeader);
    requestMessage = otCoapNewMessage(instance, &requestHeader);
    OtRtosApi_unlock();
    otEXPECT_ACTION(requestMessage != NULL, error = OT_ERROR_NO_BUFS);

    OtRtosApi_lock();
    error = otMessageAppend(requestMessage, attrTemperature,
                            strlen((const char*) attrTemperature));
    OtRtosApi_unlock();
    otEXPECT(OT_ERROR_NONE == error);

    memset(&messageInfo, 0, sizeof(messageInfo));
    otIp6AddressFromString(TIOP_OWN_REPORTING_ADDRESS, &messageInfo.mPeerAddr);
    messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;

    OtRtosApi_lock();
    error = otCoapSendRequest(instance, requestMessage, &messageInfo, NULL, NULL);
    OtRtosApi_unlock();

    /* print the reported value to the terminal */
       DISPUTILS_SERIALPRINTF(0, 0, "Message Send!");

    exit:
    if(error != OT_ERROR_NONE && requestMessage != NULL)
    {
        OtRtosApi_lock();
        otMessageFree(requestMessage);
        OtRtosApi_unlock();
    }
}

/**
 * @brief Callback function registered with the Coap server.
 *        Processes the coap request from the clients.
 *
 * @param  aContext      A pointer to the context information.
 * @param  aHeader       A pointer to the CoAP header.
 * @param  aMessage      A pointer to the message.
 * @param  aMessageInfo  A pointer to the message info.
 *
 * @return None
 */
static void coapHandleServer(void *aContext, otCoapHeader *aHeader,
                             otMessage *aMessage,
                             const otMessageInfo *aMessageInfo)
{
    otError error = OT_ERROR_NONE;
    otCoapHeader responseHeader;
    otMessage *responseMessage = NULL;
    otCoapCode responseCode = OT_COAP_CODE_CHANGED;
    otCoapCode messageCode = otCoapHeaderGetCode(aHeader);

    otCoapHeaderInit(&responseHeader, OT_COAP_TYPE_ACKNOWLEDGMENT, responseCode);
    otCoapHeaderSetMessageId(&responseHeader,
                             otCoapHeaderGetMessageId(aHeader));
    otCoapHeaderSetToken(&responseHeader, otCoapHeaderGetToken(aHeader),
                         otCoapHeaderGetTokenLength(aHeader));
    otCoapHeaderSetPayloadMarker(&responseHeader);

    if(OT_COAP_CODE_GET == messageCode)
    {
        responseMessage = otCoapNewMessage((otInstance*)aContext,
                                           &responseHeader);

        otEXPECT_ACTION(responseMessage != NULL, error = OT_ERROR_NO_BUFS);
        error = otMessageAppend(responseMessage, attrTemperature,
                                strlen((const char*) attrTemperature));
        otEXPECT(OT_ERROR_NONE == error);

        error = otCoapSendResponse((otInstance*)aContext, responseMessage,
                                   aMessageInfo);
        otEXPECT(OT_ERROR_NONE == error);
    }

exit:

    if(error != OT_ERROR_NONE && responseMessage != NULL)
    {
        otMessageFree(responseMessage);
    }
}

/**
 * @brief sets up the application coap server.
 *
 * @param aInstance A pointer to the context information.
 * @param attr      Attribute data
 *
 * @return OT_ERROR_NONE if successful, else error code
 */
static otError setupCoapServer(otInstance *aInstance, const attrDesc_t *attr)
{
    otError error = OT_ERROR_NONE;

    OtRtosApi_lock();
    error = otCoapStart(aInstance, OT_DEFAULT_COAP_PORT);
    OtRtosApi_unlock();
    otEXPECT(OT_ERROR_NONE == error);

    if(attr->type & (ATTR_READ | ATTR_WRITE))
    {
        coapResource.mHandler = &coapHandleServer;
        coapResource.mUriPath = (const char*)attr->uriPath;
        coapResource.mContext = aInstance;

        OtRtosApi_lock();
        error = otCoapAddResource(aInstance, &(coapResource));
        OtRtosApi_unlock();
        otEXPECT(OT_ERROR_NONE == error);
    }

exit:
    return error;
}

/**
 * @brief Sets the reporting address for the tempsensor task.
 *
 * @return None
 * @note The reporting address least significant byte *will* be set to
 *       @ref THERMOSTAT_ADDRESS_LSB.
 */
static void setReportingAddress(void)
{
    thermostatAddress = globalAddress;
    thermostatAddress.mFields.m8[OT_IP6_ADDRESS_SIZE - 1]
        = THERMOSTAT_ADDRESS_LSB;
    Event_post(Event_handle(&tempSensorEvents), TempSensor_evtAddressValid);
}

/**
 * @brief Initialize and construct the TIRTOS events.
 *
 * @return None
 */
static void initEvent(void)
{
    Event_construct(&tempSensorEvents, NULL);
}

/**
 * @brief Handles the key press events.
 *
 * @param keysPressed identifies which keys were pressed
 *
 * @return None
 */
static void processKeyChangeCB(uint8_t keysPressed)
{
    if (keysPressed & KEYS_RIGHT)
    {
        TempSensor_postEvt(TempSensor_evtKeyRight);
    }

   if (keysPressed & KEYS_LEFT) {
       TempSensor_postEvt(TempSensor_evtKeyLeft);
   }
}

/**
 * @brief Processes the OT stack events
 *
 * @param  evt Event identifier.
 *
 * @return None
 */
static void processOtStackEvents(uint8_t event, void *aContext)
{
    switch (event)
    {
    case OT_STACK_EVENT_NWK_JOINED:
        TempSensor_postEvt(TempSensor_evtNwkJoined);
        break;

    case OT_STACK_EVENT_NWK_JOINED_FAILURE:
        TempSensor_postEvt(TempSensor_evtNwkJoinFailure);
        break;

    case OT_STACK_EVENT_NWK_DATA_CHANGED:
        TempSensor_postEvt(TempSensor_evtNwkSetup);
        break;

    case OT_STACK_EVENT_NOTIFY_GLOBAL_ADDRESS:
        globalAddress = *((otIp6Address *) aContext);
        TempSensor_postEvt(TempSensor_evtNotifyGlobalAddress);
        break;

    default:
        // do nothing
        break;
    }
}

/**
 * @brief Processes the events.
 *
 * @return None
 */
static int processEvents(void)
{
    UInt events = Event_pend(Event_handle(&tempSensorEvents), Event_Id_NONE,
                             (TempSensor_evtReportTemp | TempSensor_evtNwkSetup |
                              TempSensor_evtAddressValid | TempSensor_evtKeyRight |
                              TempSensor_evtNwkJoined | TempSensor_evtNwkJoinFailure |
                              TempSensor_evtNotifyGlobalAddress | TempSensor_evtKeyLeft),
                             BIOS_WAIT_FOREVER);

    if(events & TempSensor_evtNwkSetup)
    {
        if (false == serverSetup)
        {
            serverSetup = true;
            (void)setupCoapServer(OtInstance_get(), &coapAttr);

            DISPUTILS_SERIALPRINTF(1, 0, "CoAP server setup done");
#ifdef TIOP_POWER_DATA_ACK
            startReportingTimer(TIOP_TEMPSENSOR_REPORTING_INTERVAL);
#endif
        }
    }

    if (events & TempSensor_evtKeyLeft) {
        sendMessage();
        DISPUTILS_SERIALPRINTF(1, 0, "Left key has been pressed!!");
    }

    if (events & TempSensor_evtKeyRight)
    {
        if ((!otDatasetIsCommissioned(OtInstance_get())) &&
            (OtStack_joinState() != OT_STACK_EVENT_NWK_JOIN_IN_PROGRESS))
        {
            DISPUTILS_SERIALPRINTF(1, 0, "Joining Nwk ...");
            OtStack_joinConfiguredNetwork();
        }
    }

    if (events & TempSensor_evtNwkJoined)
    {
        DISPUTILS_SERIALPRINTF( 1, 0, "Joined Nwk");
        (void)OtStack_setupNetwork();
    }

    if (events & TempSensor_evtNwkJoinFailure)
    {
        DISPUTILS_SERIALPRINTF(1, 0, "Join Failure");
    }

    if(events & TempSensor_evtAddressValid)
    {
        startReportingTimer(TIOP_TEMPSENSOR_REPORTING_INTERVAL);
    }

    if(events & TempSensor_evtNotifyGlobalAddress)
    {
        setReportingAddress();
    }
    return 0;
}

/**
 * Return thread priority after initialization.
 */
static void resetPriority(void)
{
    pthread_t           self;
    int                 policy;
    struct sched_param  param;
    int                 ret;

    self = pthread_self();

    ret = pthread_getschedparam(self, &policy, &param);
    assert(ret == 0);

    param.sched_priority = TASK_CONFIG_TEMPSENSOR_TASK_PRIORITY;

    ret = pthread_setschedparam(self, policy, &param);
    assert(ret == 0);
    (void)ret;
}

/******************************************************************************
 External Functions
 *****************************************************************************/

#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_APP)
/**
 * Documented in openthread/platform/logging.h.
 */
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    (void)aLogLevel;
    (void)aLogRegion;
    (void)aFormat;
    /* Do nothing. */
}
#endif

/**
 * Documented in openthread/platform/uart.h.
 */
void otPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength)
{
    (void)aBuf;
    (void)aBufLength;
    /* Do nothing. */
}

/**
 * Documented in openthread/platform/uart.h.
 */
void otPlatUartSendDone(void)
{
    /* Do nothing. */
}

/*
 * Documented in tempsensor.h
 */
void TempSensor_postEvt(TempSensor_evt event)
{
    Event_post(Event_handle(&tempSensorEvents), event);
}

/*
 * Documented in task_config.h.
 */
void TempSensor_taskCreate(void)
{
    pthread_t           thread;
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    int                 retc;

    retc = pthread_attr_init(&pAttrs);
    assert(retc == 0);

    retc = pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);
    assert(retc == 0);

    priParam.sched_priority = sched_get_priority_max(SCHED_OTHER);
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    assert(retc == 0);

    retc = pthread_attr_setstack(&pAttrs, (void *)stack,
                                 TASK_CONFIG_TEMPSENSOR_TASK_STACK_SIZE);
    assert(retc == 0);

    retc = pthread_create(&thread, &pAttrs, TempSensor_task, NULL);
    assert(retc == 0);

    retc = pthread_attr_destroy(&pAttrs);
    assert(retc == 0);
    (void)retc;
    GPIO_setConfig(Board_GPIO_RLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH |
                                       GPIO_CFG_OUT_LOW);
}


/**
 *  Temp Sensor processing thread.
 */
void *TempSensor_task(void *arg0)
{
    int ret;
#if !TIOP_CONFIG_SET_NW_ID
    bool commissioned;
#endif /* !TIOP_CONFIG_SET_NW_ID */

    otInstance *instance;
    initEvent();

    KeysUtils_initialize(processKeyChangeCB);

    OtStack_registerCallback(processOtStackEvents);

    DispUtils_open();

    AONBatMonEnable();

    resetPriority();

    instance = OtInstance_get();

    /* Set the poll period, as NVS does not store poll period */
    OtRtosApi_lock();
    otLinkSetPollPeriod(OtInstance_get(), TIOP_CONFIG_POLL_PERIOD);
    OtRtosApi_unlock();

    /* Try to bring up the network using the current dataset */
    OtStack_setupInterfaceAndNetwork();

    DISPUTILS_SERIALPRINTF(0, 0, "Temperature Sensor init!");

#if !TIOP_CONFIG_SET_NW_ID
    OtRtosApi_lock();
    commissioned = otDatasetIsCommissioned(OtInstance_get());
    OtRtosApi_unlock();
    if (false == commissioned)
    {
        otExtAddress extAddress;

        OtRtosApi_lock();
        otLinkGetFactoryAssignedIeeeEui64(OtInstance_get(), &extAddress);
        OtRtosApi_unlock();

        DISPUTILS_SERIALPRINTF(2, 0, "pskd: %s", TIOP_CONFIG_PSKD);
        DISPUTILS_SERIALPRINTF(3, 0, "EUI64: 0x%02x%02x%02x%02x%02x%02x%02x%02x",
                               extAddress.m8[0], extAddress.m8[1], extAddress.m8[2],
                               extAddress.m8[3], extAddress.m8[4], extAddress.m8[5],
                               extAddress.m8[6], extAddress.m8[7]);
    }
#endif /* !TIOP_CONFIG_SET_NW_ID */

    OtRtosApi_lock();
    otIp6AddressFromString(TIOP_TEMPSENSOR_REPORTING_ADDRESS, &thermostatAddress);
    OtRtosApi_unlock();

    configureReportingTimer();

    memset(&cli_icmpHandler, sizeof(cli_icmpHandler), 0U);
       cli_icmpHandler.mReceiveCallback = cli_icmp6RxCallback;

       OtRtosApi_lock();
       otIcmp6RegisterHandler(instance, &cli_icmpHandler);
       OtRtosApi_unlock();

    /* process events */
    while (true)
    {
        ret = processEvents();
        if (0 != ret)
        {
            break;
        }
    }
    return NULL;
}

