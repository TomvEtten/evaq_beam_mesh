/******************************************************************************

 @file tiop_config.h

 @brief TI OpenThread stack configuration parameters

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
#ifndef TIOP_CONFIG_H_
#define TIOP_CONFIG_H_

#include <openthread/instance.h>
#include <openthread/joiner.h>

/**
 * MLE Link Mode R
 */
#ifndef TIOP_CONFIG_LINK_MODE_R
#define TIOP_CONFIG_LINK_MODE_R    0
#endif

/**
 * MLE Link Mode D
 */
#ifndef TIOP_CONFIG_LINK_MODE_D
#define TIOP_CONFIG_LINK_MODE_D    0
#endif

/**
 * Data poll period for Sleepy End Devices
 */
#ifndef TIOP_CONFIG_POLL_PERIOD
#define TIOP_CONFIG_POLL_PERIOD    2000
#endif

/**
 * IEEE 802.15.4 channel
 */
#ifndef TIOP_CONFIG_LINK_CHANNEL
#define TIOP_CONFIG_LINK_CHANNEL   15
#endif

/**
 * Pre-Shared Key (PSKd) for joiner devices
 */
#ifndef TIOP_CONFIG_PSKD
#define TIOP_CONFIG_PSKD           "1337EVAQ"
#endif

/*
 * If 0, use the IEEE 802.15.4 Extended Address in ccfg.c
 * If 1, use the IEEE 802.15.4 Extended Address defined by TIOP_CONFIG_EXT_ADDR
 */
#ifndef TIOP_CONFIG_SET_EXT_ADDR
#define TIOP_CONFIG_SET_EXT_ADDR   0
#endif

/*
 * IEEE 802.15.4 Extended Address (Device ID)
 */
#ifndef TIOP_CONFIG_EXT_ADDR
#define TIOP_CONFIG_EXT_ADDR       0x0123456789ABCDEF
#endif

/*
 * If 1, set the Extended Address, PAN ID, Extended PAN ID, and Master Key using
 * the parameters defined by TIOP_CONFIG_EXT_ADDR, TIOP_CONFIG_PAN_ID,
 * TIOP_CONFIG_EXT_PAN_ID, and TIOP_CONFIG_MASTER_KEY_U/L.
 * If 0, do not set these parameters, leaving them as default. This should be used
 * when setting these parameters via the commissioning process.
 */
#ifndef TIOP_CONFIG_SET_NW_ID
#define TIOP_CONFIG_SET_NW_ID      0
#endif

/*
 * IEEE 802.15.4 PAN ID
 */
#ifndef TIOP_CONFIG_PAN_ID
#define TIOP_CONFIG_PAN_ID         0x1337
#endif

/*
 * Thread Extended PAN ID
 */
#ifndef TIOP_CONFIG_EXT_PAN_ID
#define TIOP_CONFIG_EXT_PAN_ID     0x1234567812345678
#endif

/*
 * Thread Master Key (split into upper and lower 8 bytes)
 */
#ifndef TIOP_CONFIG_MASTER_KEY_U
#define TIOP_CONFIG_MASTER_KEY_U   0x1337133713371337
#endif
#ifndef TIOP_CONFIG_MASTER_KEY_L
#define TIOP_CONFIG_MASTER_KEY_L   0x1337133713371337
#endif

/*
 * Network Name
 */
#define TIOP_CONFIG_NETWORK_NAME   "EvaqDemo"

/*
 * @brief Wrapper function around otJoinerStart for joining the network with the
 *        Pre-Shared Key (PSKd) defined in TIOP_CONFIG_PSKD
 *
 * @param  aInstance         A pointer to an OpenThread instance.
 * @param  aProvisioningUrl  A pointer to the Provisioning URL (may be NULL).
 * @param  aVendorName       A pointer to the Vendor Name (must be static).
 * @param  aVendorModel      A pointer to the Vendor Model (must be static).
 * @param  aVendorSwVersion  A pointer to the Vendor SW Version (must be static).
 * @param  aVendorData       A pointer to the Vendor Data (must be static).
 * @param  aCallback         A pointer to a function that is called when the join operation completes.
 * @param  aContext          A pointer to application-specific context.
 *
 * @retval OT_ERROR_NONE              Successfully started the Commissioner role.
 * @retval OT_ERROR_INVALID_ARGS      @p aPSKd or @p aProvisioningUrl is invalid.
 * @retval OT_ERROR_DISABLED_FEATURE  The Joiner feature is not enabled in this build.
 */
extern otError TIOP_configuredJoinerStart(otInstance *     aInstance,
                                          const char *     aProvisioningUrl,
                                          const char *     aVendorName,
                                          const char *     aVendorModel,
                                          const char *     aVendorSwVersion,
                                          const char *     aVendorData,
                                          otJoinerCallback aCallback,
                                          void *           aContext);

/*
 * @brief Configuration function which applies stack configurations via OpenThread API
 *
 * @param aInstance  A pointer to the OpenThread instance to configure.
 */
extern void TIOP_init(otInstance * aInstance);

#endif /* TIOP_CONFIG_H_ */

