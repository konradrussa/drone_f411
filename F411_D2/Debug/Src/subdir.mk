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
./Src/ahrs_common.o \
./Src/basic_math.o \
./Src/bmi160.o \
./Src/bridge_common.o \
./Src/drone.o \
./Src/init.o \
./Src/quaternion.o \
./Src/queue.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/ahrs_common.d \
./Src/basic_math.d \
./Src/bmi160.d \
./Src/bridge_common.d \
./Src/drone.d \
./Src/init.d \
./Src/quaternion.d \
./Src/queue.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ahrs_common.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/ahrs_common.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/basic_math.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/basic_math.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/bmi160.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bmi160.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/bridge_common.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/bridge_common.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/drone.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/drone.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/init.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/init.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/quaternion.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/quaternion.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/queue.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/queue.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/syscalls.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Src/sysmem.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/stm32/workspace_1.13.2/F411_FC_STATIC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ahrs_common.cyclo ./Src/ahrs_common.d ./Src/ahrs_common.o ./Src/ahrs_common.su ./Src/basic_math.cyclo ./Src/basic_math.d ./Src/basic_math.o ./Src/basic_math.su ./Src/bmi160.cyclo ./Src/bmi160.d ./Src/bmi160.o ./Src/bmi160.su ./Src/bridge_common.cyclo ./Src/bridge_common.d ./Src/bridge_common.o ./Src/bridge_common.su ./Src/drone.cyclo ./Src/drone.d ./Src/drone.o ./Src/drone.su ./Src/init.cyclo ./Src/init.d ./Src/init.o ./Src/init.su ./Src/quaternion.cyclo ./Src/quaternion.d ./Src/quaternion.o ./Src/quaternion.su ./Src/queue.cyclo ./Src/queue.d ./Src/queue.o ./Src/queue.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

