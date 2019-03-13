################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/tomvanetten/Programs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./CC26X2R1_LAUNCHXL.obj" \
"./CC26X2R1_LAUNCHXL_fxns.obj" \
"./ccfg.obj" \
"./disp_utils.obj" \
"./keys_utils.obj" \
"./main.obj" \
"./otstack.obj" \
"./tempsensor.obj" \
"./tiop_config.obj" \
"./otsupport/otrtosapi.obj" \
"./platform/alarm.obj" \
"./platform/alarm_micro.obj" \
"./platform/debug_uart.obj" \
"./platform/diag.obj" \
"./platform/misc.obj" \
"./platform/platform.obj" \
"./platform/radio.obj" \
"./platform/random.obj" \
"./platform/uart.obj" \
"./platform/crypto/aes_alt.obj" \
"./platform/crypto/crypto_misc.obj" \
"./platform/crypto/ecjpake_alt.obj" \
"./platform/crypto/sha256_alt.obj" \
"./platform/nv/crc.obj" \
"./platform/nv/nvoctp.obj" \
"./platform/nv/settings.obj" \
"../CC26X2R1_LAUNCHXL_TIRTOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibopenthread_diag_mtd.lib \
-llibopenthread_mtd.lib \
-llibopenthread_platform_utils_mtd.lib \
-llibmbedcrypto.lib \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs_cc26xx.a" \
-l"ti/drivers/rf/lib/rf_multiMode_cc26x2.aem4f" \
-l"ti/drivers/lib/drivers_cc26x2.aem4f" \
-l"ti/dpl/lib/dpl_cc26x2.aem4f" \
-l"ti/devices/cc13x2_cc26x2/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include otsupport/subdir_vars.mk
-include platform/subdir_vars.mk
-include platform/crypto/subdir_vars.mk
-include platform/nv/subdir_vars.mk
-include subdir_rules.mk
-include otsupport/subdir_rules.mk
-include platform/subdir_rules.mk
-include platform/crypto/subdir_rules.mk
-include platform/nv/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.out \

EXE_OUTPUTS__QUOTED += \
"temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.out" \

BIN_OUTPUTS += \
temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.hex \

BIN_OUTPUTS__QUOTED += \
"temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.out"

dependents:
	-cd /home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/OptimizeSize && $(MAKE) clean all

# Tool invocations
temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) /home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/OptimizeSize/libmbedcrypto.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/home/tomvanetten/Programs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O4 --opt_for_speed=0 --define=OPENTHREAD_CONFIG_FILE='"openthread-config-cc2652-ccs-mtd.h"' --define=OPENTHREAD_PROJECT_CORE_CONFIG_FILE='"openthread-core-cc2652-config-ide.h"' --define=MBEDTLS_CONFIG_FILE='"mbedtls-config-cc2652-ccs.h"' --define=HAVE_CONFIG_H --define=SIMPLELINK_OPENTHREAD_SDK_BUILD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_MTD=1 --define=SIMPLELINK_OPENTHREAD_CONFIG_CC2652=1 --define=BOARD_DISPLAY_USE_LCD=0 --define=Board_EXCLUDE_NVS_EXTERNAL_FLASH --define=NDEBUG --define=OT_STACK_IID_ADDRESS_LSB=9 --define=DeviceFamily_CC26X2 -g --c99 --gcc --printf_support=nofloat --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --gen_data_subsections=on --abi=eabi -z -m"temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.map" --heap_size=0 -i"/home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libopenthread_diag_mtd_CC26X2R1_LAUNCHXL_ccs/OptimizeSize" -i"/home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libopenthread_mtd_CC26X2R1_LAUNCHXL_ccs/OptimizeSize" -i"/home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libopenthread_platform_utils_mtd_CC26X2R1_LAUNCHXL_ccs/OptimizeSize" -i"/home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/OptimizeSize" -i"/home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source" -i"/home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages" -i"/home/tomvanetten/Programs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs_linkInfo.xml" --rom_model --unused_section_elimination=on -o "temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/home/tomvanetten/Programs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armhex"  -o "temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CC26X2R1_LAUNCHXL.obj" "CC26X2R1_LAUNCHXL_fxns.obj" "ccfg.obj" "disp_utils.obj" "keys_utils.obj" "main.obj" "otstack.obj" "tempsensor.obj" "tiop_config.obj" "otsupport/otrtosapi.obj" "platform/alarm.obj" "platform/alarm_micro.obj" "platform/debug_uart.obj" "platform/diag.obj" "platform/misc.obj" "platform/platform.obj" "platform/radio.obj" "platform/random.obj" "platform/uart.obj" "platform/crypto/aes_alt.obj" "platform/crypto/crypto_misc.obj" "platform/crypto/ecjpake_alt.obj" "platform/crypto/sha256_alt.obj" "platform/nv/crc.obj" "platform/nv/nvoctp.obj" "platform/nv/settings.obj" 
	-$(RM) "CC26X2R1_LAUNCHXL.d" "CC26X2R1_LAUNCHXL_fxns.d" "ccfg.d" "disp_utils.d" "keys_utils.d" "main.d" "otstack.d" "tempsensor.d" "tiop_config.d" "otsupport/otrtosapi.d" "platform/alarm.d" "platform/alarm_micro.d" "platform/debug_uart.d" "platform/diag.d" "platform/misc.d" "platform/platform.d" "platform/radio.d" "platform/random.d" "platform/uart.d" "platform/crypto/aes_alt.d" "platform/crypto/crypto_misc.d" "platform/crypto/ecjpake_alt.d" "platform/crypto/sha256_alt.d" "platform/nv/crc.d" "platform/nv/nvoctp.d" "platform/nv/settings.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/tomvanetten/Documents/Workspace/evaq_beam_mesh/libmbedcrypto_CC26X2R1_LAUNCHXL_ccs/OptimizeSize/libmbedcrypto.lib:

-include ../makefile.targets

