################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O4 --opt_for_speed=0 --include_path="C:/Users/Tom van Etten/workspace_v8/libopenthread_ftd_CC26X2R1_LAUNCHXL_ccs/config" --include_path="C:/Users/Tom van Etten/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/config" --include_path="C:/Users/Tom van Etten/workspace_v8/cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/examples/platforms" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/include" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/src/core" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/src/cli" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/third_party/mbedtls/repo/include" --include_path="C:/Users/Tom van Etten/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs" --include_path="C:/Users/Tom van Etten/workspace_v8/cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs/platform/crypto" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/ti/posix/ccs" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=OPENTHREAD_CONFIG_FILE='"openthread-config-cc2652-ccs-ftd.h"' --define=OPENTHREAD_PROJECT_CORE_CONFIG_FILE='"openthread-core-cc2652-config-ide.h"' --define=MBEDTLS_CONFIG_FILE='"mbedtls-config-cc2652-ccs.h"' --define=HAVE_CONFIG_H --define=SIMPLELINK_OPENTHREAD_SDK_BUILD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_FTD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_CC2652=1 --define=Board_EXCLUDE_NVS_EXTERNAL_FLASH --define=TIOP_ENABLE_UART=1 --define=NDEBUG --define=DeviceFamily_CC26X2 -g --c99 --gcc --printf_support=nofloat --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --gen_data_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-695126415:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-695126415-inproc

build-695126415-inproc: ../release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_51_01_18_core/xs" --xdcpath="C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source;C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC2652R1F -r release -c "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O4 --opt_for_speed=0 --include_path=\"C:/Users/Tom van Etten/workspace_v8/libopenthread_ftd_CC26X2R1_LAUNCHXL_ccs/config\" --include_path=\"C:/Users/Tom van Etten/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/config\" --include_path=\"C:/Users/Tom van Etten/workspace_v8/cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/examples/platforms\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/include\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/src/core\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/src/cli\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/third_party/openthread/third_party/mbedtls/repo/include\" --include_path=\"C:/Users/Tom van Etten/workspace_v8/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs\" --include_path=\"C:/Users/Tom van Etten/workspace_v8/cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs/platform/crypto\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source/ti/posix/ccs\" --include_path=\"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include\" --define=OPENTHREAD_CONFIG_FILE='\"openthread-config-cc2652-ccs-ftd.h\"' --define=OPENTHREAD_PROJECT_CORE_CONFIG_FILE='\"openthread-core-cc2652-config-ide.h\"' --define=MBEDTLS_CONFIG_FILE='\"mbedtls-config-cc2652-ccs.h\"' --define=HAVE_CONFIG_H --define=SIMPLELINK_OPENTHREAD_SDK_BUILD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_FTD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_CC2652=1 --define=Board_EXCLUDE_NVS_EXTERNAL_FLASH --define=TIOP_ENABLE_UART=1 --define=NDEBUG --define=DeviceFamily_CC26X2 -g --c99 --gcc --printf_support=nofloat --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --gen_data_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-695126415 ../release.cfg
configPkg/compiler.opt: build-695126415
configPkg/: build-695126415

