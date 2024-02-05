################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/calibration.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/tests.c \
D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/unity.c 

OBJS += \
./Toolkit_Src/calibration.o \
./Toolkit_Src/tests.o \
./Toolkit_Src/unity.o 

C_DEPS += \
./Toolkit_Src/calibration.d \
./Toolkit_Src/tests.d \
./Toolkit_Src/unity.d 


# Each subdirectory must supply rules for building sources it contributes
Toolkit_Src/calibration.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/calibration.c Toolkit_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Toolkit_Src/tests.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/tests.c Toolkit_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Toolkit_Src/unity.o: D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Src/unity.c Toolkit_Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_TOOLKIT/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Toolkit_Src

clean-Toolkit_Src:
	-$(RM) ./Toolkit_Src/calibration.cyclo ./Toolkit_Src/calibration.d ./Toolkit_Src/calibration.o ./Toolkit_Src/calibration.su ./Toolkit_Src/tests.cyclo ./Toolkit_Src/tests.d ./Toolkit_Src/tests.o ./Toolkit_Src/tests.su ./Toolkit_Src/unity.cyclo ./Toolkit_Src/unity.d ./Toolkit_Src/unity.o ./Toolkit_Src/unity.su

.PHONY: clean-Toolkit_Src

