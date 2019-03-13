################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
platform/crypto/%.obj: ../platform/crypto/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O4 --opt_for_speed=0 --include_path="/Users/rick/workspace_v8/libopenthread_mtd_CC26X2R1_LAUNCHXL_ccs/config" --include_path="/Users/rick/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/config" --include_path="/Users/rick/workspace_v8/temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/examples/platforms" --include_path="/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/include" --include_path="/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/src/core" --include_path="/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/third_party/mbedtls/repo/include" --include_path="/Users/rick/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs" --include_path="/Users/rick/workspace_v8/temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs/platform/crypto" --include_path="/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/ti/posix/ccs" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=OPENTHREAD_CONFIG_FILE='"openthread-config-cc2652-ccs-mtd.h"' --define=OPENTHREAD_PROJECT_CORE_CONFIG_FILE='"openthread-core-cc2652-config-ide.h"' --define=MBEDTLS_CONFIG_FILE='"mbedtls-config-cc2652-ccs.h"' --define=HAVE_CONFIG_H --define=SIMPLELINK_OPENTHREAD_SDK_BUILD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_MTD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_CC2652=1 --define=BOARD_DISPLAY_USE_LCD=0 --define=Board_EXCLUDE_NVS_EXTERNAL_FLASH --define=NDEBUG --define=OT_STACK_IID_ADDRESS_LSB=9 --define=DeviceFamily_CC26X2 -g --c99 --gcc --printf_support=nofloat --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --gen_data_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="platform/crypto/$(basename $(<F)).d_raw" --obj_directory="platform/crypto" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


