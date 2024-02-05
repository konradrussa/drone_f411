################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/calibration.c \
../Src/tests.c \
../Src/unity.c 

OBJS += \
./Src/calibration.o \
./Src/tests.o \
./Src/unity.o 

C_DEPS += \
./Src/calibration.d \
./Src/tests.d \
./Src/unity.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/calibration.cyclo ./Src/calibration.d ./Src/calibration.o ./Src/calibration.su ./Src/tests.cyclo ./Src/tests.d ./Src/tests.o ./Src/tests.su ./Src/unity.cyclo ./Src/unity.d ./Src/unity.o ./Src/unity.su

.PHONY: clean-Src

