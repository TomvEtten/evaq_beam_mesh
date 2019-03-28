/******************************************************************************

 @file otstack.c

 @brief OpenThread stack processing and instantiation and handling of
        application CoAP server.

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
#include OPENTHREAD_PROJECT_CORE_CONFIG_FILE

/* Standard Library Header files */
#include <assert.h>
#include <stddef.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <sched.h>

/* RTOS header files */
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Event.h>

/* OpenThread public API Header files */
#include <openthread/coap.h>
#include <openthread/dataset.h>
#include <openthread/diag.h>
#include <openthread/joiner.h>
#include <openthread/platform/settings.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/ip6.h>
/* OpenThread Internal/Example Header files */
#include "otsupport/otinstance.h"
#include "otsupport/otrtosapi.h"
#include "platform/platform.h"

/* Example/Board Header files */
#include "Board.h"
#include "otstack.h"
#include "task_config.h"
#include "utils/code_utils.h"

/* Configuration Header files */
#include "tiop_config.h"

/* OpenThread Stack thread */
extern void *OtStack_task(void *arg0);

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

#define OT_STACK_EVENT_SIGNAL_ALARM_PROCESS   Event_Id_00
#define OT_STACK_EVENT_SIGNAL_RADIO_PROCESS   Event_Id_01
#define OT_STACK_EVENT_SIGNAL_TASLETS_PENDING Event_Id_02
#define OT_STACK_EVENT_SIGNAL_UART_PROCESS    Event_Id_03
#define OT_STACK_EVENT_SIGNAL_RANDOM_PROCESS  Event_Id_04
#define OT_STACK_EVENT_SIGNAL_ALARMU_PROCESS  Event_Id_05
#define OT_STACK_EVENT_SIGNAL_SPI_PROCESS     Event_Id_06

/******************************************************************************
 Local variables
 *****************************************************************************/

static otInstance *OtStack_instance;

/* TI-RTOS events structure for passing state to the processing loop */
static Event_Struct OtStack_events;

/* OpenThread Stack thread call stack */
static char OtStack_stack[TASK_CONFIG_OT_TASK_STACK_SIZE];

/* Pointer to application callback */
static OtStack_EventsCallback_t appEventHandler = NULL;

static otNetifAddress addresses[OT_STACK_MAX_ADDRESSES];

/* Holds the stack events related to network */
static volatile uint8_t otStackEvents = OT_STACK_EVENT_NWK_NOT_JOINED;

/******************************************************************************
 Local Functions
 *****************************************************************************/



/**
 * @brief callback function registered with the OpenThread to
 *        get the joining network status.
 *
 * @param aError   error value returned during the joining
 *                 process.
 * @param aContext context if any (ignored).
 * @return None
 */
void joinerCallback(otError aError, void *aContext)
{
    (void)aContext;

    if(aError == OT_ERROR_NONE)
    {
        otStackEvents = OT_STACK_EVENT_NWK_JOINED;
    }
    else
    {
        otStackEvents = OT_STACK_EVENT_NWK_JOINED_FAILURE;
    }

    if (appEventHandler)
    {
        appEventHandler(otStackEvents, NULL);
    }
}

/******************************************************************************
 External Functions
 *****************************************************************************/

/* Documented in otstack.h */
void handleNetifStateChanged(uint32_t aFlags, void *aContext)
{
    otInstance *aInstance = (otInstance *)aContext;

    if(aFlags & OT_CHANGED_THREAD_NETDATA)
    {
        otIp6SlaacUpdate(aInstance, addresses,
                         sizeof(addresses) / sizeof(addresses[0]),
                         otIp6CreateMacIid, NULL);

        /* post the network setup done event to the resgistered app */
        otStackEvents = OT_STACK_EVENT_NWK_DATA_CHANGED;
        appEventHandler(otStackEvents, NULL);
    }

    if(aFlags & OT_CHANGED_THREAD_ROLE)
    {
        otDeviceRole role = otThreadGetDeviceRole(aInstance);
#ifndef TIOP_POWER_MEASUREMENT
        switch (role)
        {
            case OT_DEVICE_ROLE_DISABLED:
                GPIO_write(Board_GPIO_GLED, Board_GPIO_LED_OFF);
                GPIO_write(Board_GPIO_RLED, Board_GPIO_LED_OFF);
                break;

            case OT_DEVICE_ROLE_DETACHED:
                GPIO_write(Board_GPIO_GLED, Board_GPIO_LED_OFF);
                GPIO_write(Board_GPIO_RLED, Board_GPIO_LED_ON);
                break;

            case OT_DEVICE_ROLE_CHILD:
                // Fall Through
            case OT_DEVICE_ROLE_ROUTER:
                GPIO_write(Board_GPIO_GLED, Board_GPIO_LED_ON);
                GPIO_write(Board_GPIO_RLED, Board_GPIO_LED_OFF);
                break;

            case OT_DEVICE_ROLE_LEADER:
                GPIO_write(Board_GPIO_GLED, Board_GPIO_LED_ON);
                GPIO_write(Board_GPIO_RLED, Board_GPIO_LED_ON);
                break;

            default:
                break;
        }
#endif  /* !TIOP_POWER_MEASUREMENT */
        /* post the network setup done event to the registered app */
        otStackEvents = OT_STACK_EVENT_DEV_ROLE_CHANGED;
        appEventHandler(otStackEvents, NULL);
    }
    return;
}


/* Documented in otstack.h */
void OtStack_registerCallback(OtStack_EventsCallback_t appCB)
{
    appEventHandler = appCB;
}

/* Documented in otstack.h */
uint8_t OtStack_joinState(void)
{
    return otStackEvents;
}

/* Documented in otstack.h */
void OtStack_joinNetwork(const char* pskd)
{
    OtRtosApi_lock();
    otIp6SetEnabled(OtStack_instance, true);
    if (OT_ERROR_NONE == otJoinerStart(OtStack_instance, pskd, NULL, PACKAGE_NAME, OPENTHREAD_CONFIG_PLATFORM_INFO,
                  PACKAGE_VERSION, NULL, joinerCallback, NULL))
    {
        otStackEvents = OT_STACK_EVENT_NWK_JOIN_IN_PROGRESS;
    }
    OtRtosApi_unlock();
}

/* Documented in otstack.h */
void OtStack_joinConfiguredNetwork(void)
{
    OtRtosApi_lock();
    otIp6SetEnabled(OtStack_instance, true);
    if (OT_ERROR_NONE == TIOP_configuredJoinerStart(OtStack_instance, NULL, PACKAGE_NAME,
                  OPENTHREAD_CONFIG_PLATFORM_INFO, PACKAGE_VERSION, NULL, joinerCallback, NULL))
    {
        otStackEvents = OT_STACK_EVENT_NWK_JOIN_IN_PROGRESS;
    }
    OtRtosApi_unlock();
}

/* Documented in otstack.h */
bool OtStack_setupNetwork(void)
{
    bool status = false;
    OtRtosApi_lock();
    if (otIp6IsEnabled(OtStack_instance))
    {
        /* Only try to start Thread if we could bring up the interface */
        if (otThreadSetEnabled(OtStack_instance, true) == OT_ERROR_NONE)
        {
            status = true;
            /* Register the network interface state change callback */
            otSetStateChangedCallback(OtStack_instance, handleNetifStateChanged,
                                      (void *)OtStack_instance);
        }
    }
    OtRtosApi_unlock();
    return status;
}

/* Documented in otstack.h */
bool OtStack_setupInterfaceAndNetwork(void)
{
    bool status = false;
    OtRtosApi_lock();
    if (otIp6SetEnabled(OtInstance_get(), true) == OT_ERROR_NONE)
    {
        // Only try to start Thread if we could bring up the interface
        if (otThreadSetEnabled(OtInstance_get(), true) != OT_ERROR_NONE)
        {
            /* Bring the interface down if Thread failed to start */
            otIp6SetEnabled(OtInstance_get(), false);
        }
        else
        {
            status = true;
            otSetStateChangedCallback(OtInstance_get(), handleNetifStateChanged,
                                      (void *)OtInstance_get());
        }
    }
    OtRtosApi_unlock();
    return status;
}

/**
 * Documented in task_config.h.
 */
void OtStack_taskCreate(void)
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

    retc = pthread_attr_setstack(&pAttrs, (void *)OtStack_stack,
                                 TASK_CONFIG_OT_TASK_STACK_SIZE);
    assert(retc == 0);

    retc = pthread_create(&thread, &pAttrs, OtStack_task, NULL);
    assert(retc == 0);

    retc = pthread_attr_destroy(&pAttrs);
    assert(retc == 0);

    (void) retc;

    GPIO_setConfig(Board_GPIO_BTN1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_BTN2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_RLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH |
                                    GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_GLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH |
                                    GPIO_CFG_OUT_LOW);
}

/**
 * Callback from OpenThread stack to indicate tasklets are pending processing.
 */
void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_TASLETS_PENDING);
}

/**
 * Callback from the alarm module indicating need for processing.
 */
void platformAlarmSignal()
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_ALARM_PROCESS);
}

/**
 * Callback from the alarm module indicating need for processing.
 */
void platformAlarmMicroSignal()
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_ALARMU_PROCESS);
}

/**
 * Callback from the alarm module indicating need for processing.
 */
void platformUartSignal()
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_UART_PROCESS);
}

/**
 * Callback from the spi module indicating need for processing.
 */
void platformSpiSignal()
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_SPI_PROCESS);
}

/**
 * Callback from the radio module indicating need for processing.
 */
void platformRadioSignal()
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_RADIO_PROCESS);
}

/**
 * Callback from the random generator module indicating need for
 * processing.
 */
void platformRandomSignal(void)
{
    Event_post(Event_handle(&OtStack_events), OT_STACK_EVENT_SIGNAL_RANDOM_PROCESS);
}


/**
 * Documented in otsupport/otinstance.h.
 */
otInstance *OtInstance_get(void)
{
    return OtStack_instance;
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

    param.sched_priority = TASK_CONFIG_OT_TASK_PRIORITY;

    ret = pthread_setschedparam(self, policy, &param);
    assert(ret == 0);

    (void) ret;
}

/**
 * Main processing thread for OpenThread Stack.
 */
void *OtStack_task(void *arg0)
{
    /* Initialize the processing loop event structure */
    Event_construct(&OtStack_events, NULL);

    /* Initialize the platform */
    PlatformInit(0, NULL);

    OtRtosApi_init();

    OtStack_instance = otInstanceInitSingle();
    assert(OtStack_instance);

#if OPENTHREAD_ENABLE_DIAG
    otDiagInit(OtStack_instance);
#endif

#ifndef TIOP_POWER_MEASUREMENT
    /* If both buttons are pressed on boot, reset the OpenThread settings */
    if(GPIO_read(Board_GPIO_BTN1) == 0U && GPIO_read(Board_GPIO_BTN2) == 0U)
    {
        otInstanceFactoryReset(OtStack_instance);
    }
#endif /* !TIOP_POWER_MEASUREMENT */

    /* Set the SysCfg params if the dataset has not been commissioned. */
    OtRtosApi_lock();
    if (!otDatasetIsCommissioned(OtStack_instance))
    {
        TIOP_init(OtStack_instance);
    }
    OtRtosApi_unlock();

    resetPriority();

    while (1)
    {
        UInt events;

        events = Event_pend(Event_handle(&OtStack_events),
                            Event_Id_NONE,
                            (OT_STACK_EVENT_SIGNAL_ALARM_PROCESS
                             | OT_STACK_EVENT_SIGNAL_ALARMU_PROCESS
                             | OT_STACK_EVENT_SIGNAL_RADIO_PROCESS
                             | OT_STACK_EVENT_SIGNAL_TASLETS_PENDING
                             | OT_STACK_EVENT_SIGNAL_UART_PROCESS
                             | OT_STACK_EVENT_SIGNAL_SPI_PROCESS
                             | OT_STACK_EVENT_SIGNAL_RANDOM_PROCESS),
                            BIOS_WAIT_FOREVER);

        if (events & OT_STACK_EVENT_SIGNAL_ALARM_PROCESS)
        {
            OtRtosApi_lock();
            platformAlarmProcess(OtStack_instance);
            OtRtosApi_unlock();
        }
#if OPENTHREAD_CONFIG_ENABLE_PLATFORM_USEC_TIMER
        if (events & OT_STACK_EVENT_SIGNAL_ALARMU_PROCESS)
        {
            OtRtosApi_lock();
            platformAlarmMicroProcess(OtStack_instance);
            OtRtosApi_unlock();
        }
#endif
        if (events & OT_STACK_EVENT_SIGNAL_RADIO_PROCESS)
        {
            OtRtosApi_lock();
            platformRadioProcess(OtStack_instance);
            OtRtosApi_unlock();
        }

        if (events & OT_STACK_EVENT_SIGNAL_TASLETS_PENDING)
        {
            OtRtosApi_lock();
            otTaskletsProcess(OtStack_instance);
            OtRtosApi_unlock();
        }
#if OPENTHREAD_ENABLE_NCP_UART || TIOP_ENABLE_UART
        if (events & OT_STACK_EVENT_SIGNAL_UART_PROCESS)
        {
            OtRtosApi_lock();
            platformUartProcess();
            OtRtosApi_unlock();
        }
#endif
#if OPENTHREAD_ENABLE_NCP_SPI
        if (events & OT_STACK_EVENT_SIGNAL_SPI_PROCESS)
        {
            OtRtosApi_lock();
            platformSpiProcess();
            OtRtosApi_unlock();
        }
#endif
        if (events & OT_STACK_EVENT_SIGNAL_RANDOM_PROCESS)
        {
            OtRtosApi_lock();
            platformRandomProcess();
            OtRtosApi_unlock();
        }

    }
}

