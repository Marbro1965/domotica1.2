################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Source/HelperClasses/ini.obj: ../Source/HelperClasses/ini.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.2.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Source/HelperClasses/ini.d" --obj_directory="Source/HelperClasses" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


