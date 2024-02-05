################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ahrs_common.c \
../Src/basic_math.c \
../Src/bmi160.c \
../Src/bridge_common.c \
../Src/drone.c \
../Src/env.c \
../Src/flight_control_common.c \
../Src/init.c \
../Src/quaternion.c \
../Src/queue.c \
../Src/timer.c 

OBJS += \
./Src/ahrs_common.o \
./Src/basic_math.o \
./Src/bmi160.o \
./Src/bridge_common.o \
./Src/drone.o \
./Src/env.o \
./Src/flight_control_common.o \
./Src/init.o \
./Src/quaternion.o \
./Src/queue.o \
./Src/timer.o 

C_DEPS += \
./Src/ahrs_common.d \
./Src/basic_math.d \
./Src/bmi160.d \
./Src/bridge_common.d \
./Src/drone.d \
./Src/env.d \
./Src/flight_control_common.d \
./Src/init.d \
./Src/quaternion.d \
./Src/queue.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ahrs_common.cyclo ./Src/ahrs_common.d ./Src/ahrs_common.o ./Src/ahrs_common.su ./Src/basic_math.cyclo ./Src/basic_math.d ./Src/basic_math.o ./Src/basic_math.su ./Src/bmi160.cyclo ./Src/bmi160.d ./Src/bmi160.o ./Src/bmi160.su ./Src/bridge_common.cyclo ./Src/bridge_common.d ./Src/bridge_common.o ./Src/bridge_common.su ./Src/drone.cyclo ./Src/drone.d ./Src/drone.o ./Src/drone.su ./Src/env.cyclo ./Src/env.d ./Src/env.o ./Src/env.su ./Src/flight_control_common.cyclo ./Src/flight_control_common.d ./Src/flight_control_common.o ./Src/flight_control_common.su ./Src/init.cyclo ./Src/init.d ./Src/init.o ./Src/init.su ./Src/quaternion.cyclo ./Src/quaternion.d ./Src/quaternion.o ./Src/quaternion.su ./Src/queue.cyclo ./Src/queue.d ./Src/queue.o ./Src/queue.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su

.PHONY: clean-Src

