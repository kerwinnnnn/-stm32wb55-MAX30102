################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/ble/core/template/osal.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/ble/core/template/osal.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/ble/core/template/osal.d 


# 每个子目录必须为构建它所贡献的源提供规则
Middlewares/ST/STM32_WPAN/ble/core/template/%.o Middlewares/ST/STM32_WPAN/ble/core/template/%.su Middlewares/ST/STM32_WPAN/ble/core/template/%.cyclo: ../Middlewares/ST/STM32_WPAN/ble/core/template/%.c Middlewares/ST/STM32_WPAN/ble/core/template/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I"E:/stm32cubeide_workspace/BLE_PROJECT/APP" -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-template

clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-template:
	-$(RM) ./Middlewares/ST/STM32_WPAN/ble/core/template/osal.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/template/osal.d ./Middlewares/ST/STM32_WPAN/ble/core/template/osal.o ./Middlewares/ST/STM32_WPAN/ble/core/template/osal.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-template

