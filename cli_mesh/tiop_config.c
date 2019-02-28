/******************************************************************************

 @file tiop_config.c

 @brief TI-OpenThread stack configuration function implementation

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
/* Standard Library header files */
#include <assert.h>

/* OpenThread public API header files */
#include <openthread/instance.h>
#include <openthread/thread.h>
#include <openthread/link.h>
#include <openthread/joiner.h>

/* Configuration header file */
#include "tiop_config.h"


/**
 * Documented in tiop_config.h
 */
otError TIOP_configuredJoinerStart(otInstance *     aInstance,
                                   const char *     aProvisioningUrl,
                                   const char *     aVendorName,
                                   const char *     aVendorModel,
                                   const char *     aVendorSwVersion,
                                   const char *     aVendorData,
                                   otJoinerCallback aCallback,
                                   void *           aContext)
{
    return otJoinerStart(aInstance, TIOP_CONFIG_PSKD, aProvisioningUrl, aVendorName,
                         aVendorModel, aVendorSwVersion, aVendorData, aCallback, aContext);
}

/**
 * Documented in tiop_config.h
 */
void TIOP_init(otInstance * aInstance)
{
    unsigned int i;
    otError err;

    otLinkModeConfig linkMode;
    otExtAddress extAddress;
    otExtendedPanId extPanID;
    otMasterKey aKey;


    /* Set link mode */
    linkMode.mRxOnWhenIdle       = TIOP_CONFIG_LINK_MODE_R;
    linkMode.mSecureDataRequests = 1;
    linkMode.mDeviceType         = TIOP_CONFIG_LINK_MODE_D;
    linkMode.mNetworkData        = 1;
    err = otThreadSetLinkMode(aInstance, linkMode);
    assert(err == OT_ERROR_NONE);

    /* Set poll period */
    if (linkMode.mRxOnWhenIdle == 0)
    {
        otLinkSetPollPeriod(aInstance, TIOP_CONFIG_POLL_PERIOD);
    }

    /* Set default channel */
    err = otLinkSetChannel(aInstance, TIOP_CONFIG_LINK_CHANNEL);
    assert(err == OT_ERROR_NONE);

    /* Set extended address (device ID) if TIOP_CONFIG_SET_EXT_ADDR is 1 */
    if (TIOP_CONFIG_SET_EXT_ADDR)
    {
        for (i = 0; i < OT_EXT_ADDRESS_SIZE; i++)
        {
            extAddress.m8[OT_EXT_ADDRESS_SIZE - i - 1] =
                    ((TIOP_CONFIG_EXT_ADDR >> (8*i)) & 0xFF);
        }
        err = otLinkSetExtendedAddress(aInstance, &extAddress);
        assert(err == OT_ERROR_NONE);
    }

    /* Set network ID parameters if TIOP_CONFIG_SET_NW_ID is 1 */
    if (TIOP_CONFIG_SET_NW_ID)
    {
        /* Set PAN ID */
        err = otLinkSetPanId(aInstance, TIOP_CONFIG_PAN_ID);
        assert(err == OT_ERROR_NONE);

        /* Set extended PAN ID */
        for (i = 0; i < OT_EXT_PAN_ID_SIZE; i++)
        {
            extPanID.m8[OT_EXT_PAN_ID_SIZE - i - 1] =
                    ((TIOP_CONFIG_EXT_PAN_ID >> (8*i)) & 0xFF);
        }
        err = otThreadSetExtendedPanId(aInstance, &extPanID);
        assert(err == OT_ERROR_NONE);

        /* Set master key*/
        for (i = 0; i < OT_MASTER_KEY_SIZE/2; i++)
        {
            aKey.m8[OT_MASTER_KEY_SIZE - i - 1] =
                    ((TIOP_CONFIG_MASTER_KEY_L >> (8*i)) & 0xFF);
        }
        for (i = OT_MASTER_KEY_SIZE/2; i < OT_MASTER_KEY_SIZE; i++)
        {
            aKey.m8[OT_MASTER_KEY_SIZE - i - 1] =
                    ((TIOP_CONFIG_MASTER_KEY_U >> (8*(i-(OT_MASTER_KEY_SIZE/2)))) & 0xFF);
        }
        err = otThreadSetMasterKey(aInstance, (const otMasterKey *) (&aKey));
        assert(err == OT_ERROR_NONE);

        /* Set network name */
        err = otThreadSetNetworkName(aInstance, TIOP_CONFIG_NETWORK_NAME);
        assert(err == OT_ERROR_NONE);
    }

    /* Prevent compiler warning */
    (void) err;
}

