################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/ahrs_common.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/basic_math.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bmi160.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bridge_common.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/drone.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/init.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/quaternion.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/queue.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/syscalls.c \
D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/sysmem.c 

OBJS += \
./Core/Static_Src/ahrs_common.o \
./Core/Static_Src/basic_math.o \
./Core/Static_Src/bmi160.o \
./Core/Static_Src/bridge_common.o \
./Core/Static_Src/drone.o \
./Core/Static_Src/init.o \
./Core/Static_Src/quaternion.o \
./Core/Static_Src/queue.o \
./Core/Static_Src/syscalls.o \
./Core/Static_Src/sysmem.o 

C_DEPS += \
./Core/Static_Src/ahrs_common.d \
./Core/Static_Src/basic_math.d \
./Core/Static_Src/bmi160.d \
./Core/Static_Src/bridge_common.d \
./Core/Static_Src/drone.d \
./Core/Static_Src/init.d \
./Core/Static_Src/quaternion.d \
./Core/Static_Src/queue.d \
./Core/Static_Src/syscalls.d \
./Core/Static_Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Static_Src/ahrs_common.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/ahrs_common.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/basic_math.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/basic_math.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/bmi160.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bmi160.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/bridge_common.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bridge_common.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/drone.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/drone.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/init.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/init.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/quaternion.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/quaternion.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/queue.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/queue.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/syscalls.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/syscalls.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Static_Src/sysmem.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/sysmem.c Core/Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Static_Src

clean-Core-2f-Static_Src:
	-$(RM) ./Core/Static_Src/ahrs_common.cyclo ./Core/Static_Src/ahrs_common.d ./Core/Static_Src/ahrs_common.o ./Core/Static_Src/ahrs_common.su ./Core/Static_Src/basic_math.cyclo ./Core/Static_Src/basic_math.d ./Core/Static_Src/basic_math.o ./Core/Static_Src/basic_math.su ./Core/Static_Src/bmi160.cyclo ./Core/Static_Src/bmi160.d ./Core/Static_Src/bmi160.o ./Core/Static_Src/bmi160.su ./Core/Static_Src/bridge_common.cyclo ./Core/Static_Src/bridge_common.d ./Core/Static_Src/bridge_common.o ./Core/Static_Src/bridge_common.su ./Core/Static_Src/drone.cyclo ./Core/Static_Src/drone.d ./Core/Static_Src/drone.o ./Core/Static_Src/drone.su ./Core/Static_Src/init.cyclo ./Core/Static_Src/init.d ./Core/Static_Src/init.o ./Core/Static_Src/init.su ./Core/Static_Src/quaternion.cyclo ./Core/Static_Src/quaternion.d ./Core/Static_Src/quaternion.o ./Core/Static_Src/quaternion.su ./Core/Static_Src/queue.cyclo ./Core/Static_Src/queue.d ./Core/Static_Src/queue.o ./Core/Static_Src/queue.su ./Core/Static_Src/syscalls.cyclo ./Core/Static_Src/syscalls.d ./Core/Static_Src/syscalls.o ./Core/Static_Src/syscalls.su ./Core/Static_Src/sysmem.cyclo ./Core/Static_Src/sysmem.d ./Core/Static_Src/sysmem.o ./Core/Static_Src/sysmem.su

.PHONY: clean-Core-2f-Static_Src

