################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# 此处定义了参与生成的所有源
-include sources.mk
-include Utilities/sequencer/subdir.mk
-include Utilities/lpm/tiny_lpm/subdir.mk
-include STM32_WPAN/Target/subdir.mk
-include STM32_WPAN/App/subdir.mk
-include Middlewares/ST/STM32_WPAN/utilities/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/svc/Src/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/template/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/auto/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include APP/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := BLE_PROJECT
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# 将这些工具调用的输入和输出添加到构建变量 
EXECUTABLES += \
BLE_PROJECT.elf \

MAP_FILES += \
BLE_PROJECT.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BLE_PROJECT.list \


# 所有目标
all: main-build

# 主构建目标
main-build: BLE_PROJECT.elf secondary-outputs

# 工具调用
BLE_PROJECT.elf BLE_PROJECT.map: $(OBJS) $(USER_OBJS) E:\stm32cubeide_workspace\BLE_PROJECT\STM32WB55RGVX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "BLE_PROJECT.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\stm32cubeide_workspace\BLE_PROJECT\STM32WB55RGVX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BLE_PROJECT.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo '已结束构建目标： $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo '已结束构建： $@'
	@echo ' '

BLE_PROJECT.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BLE_PROJECT.list"
	@echo '已结束构建： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) BLE_PROJECT.elf BLE_PROJECT.list BLE_PROJECT.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
