################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/ahrs_common.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/basic_math.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/bmi160.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/bridge_common.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/drone.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/init.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/quaternion.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/queue.c 

OBJS += \
./Static_Src/ahrs_common.o \
./Static_Src/basic_math.o \
./Static_Src/bmi160.o \
./Static_Src/bridge_common.o \
./Static_Src/drone.o \
./Static_Src/init.o \
./Static_Src/quaternion.o \
./Static_Src/queue.o 

C_DEPS += \
./Static_Src/ahrs_common.d \
./Static_Src/basic_math.d \
./Static_Src/bmi160.d \
./Static_Src/bridge_common.d \
./Static_Src/drone.d \
./Static_Src/init.d \
./Static_Src/quaternion.d \
./Static_Src/queue.d 


# Each subdirectory must supply rules for building sources it contributes
Static_Src/ahrs_common.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/ahrs_common.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/basic_math.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/basic_math.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/bmi160.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/bmi160.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/bridge_common.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/bridge_common.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/drone.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/drone.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/init.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/init.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/quaternion.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/quaternion.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Static_Src/queue.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src/queue.c Static_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Static_Src

clean-Static_Src:
	-$(RM) ./Static_Src/ahrs_common.cyclo ./Static_Src/ahrs_common.d ./Static_Src/ahrs_common.o ./Static_Src/ahrs_common.su ./Static_Src/basic_math.cyclo ./Static_Src/basic_math.d ./Static_Src/basic_math.o ./Static_Src/basic_math.su ./Static_Src/bmi160.cyclo ./Static_Src/bmi160.d ./Static_Src/bmi160.o ./Static_Src/bmi160.su ./Static_Src/bridge_common.cyclo ./Static_Src/bridge_common.d ./Static_Src/bridge_common.o ./Static_Src/bridge_common.su ./Static_Src/drone.cyclo ./Static_Src/drone.d ./Static_Src/drone.o ./Static_Src/drone.su ./Static_Src/init.cyclo ./Static_Src/init.d ./Static_Src/init.o ./Static_Src/init.su ./Static_Src/quaternion.cyclo ./Static_Src/quaternion.d ./Static_Src/quaternion.o ./Static_Src/quaternion.su ./Static_Src/queue.cyclo ./Static_Src/queue.d ./Static_Src/queue.o ./Static_Src/queue.su

.PHONY: clean-Static_Src

