################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ahrs.c \
../Core/Src/bmm150.c \
../Core/Src/bmp2.c \
../Core/Src/bmp280.c \
../Core/Src/bridge.c \
../Core/Src/buzzer.c \
../Core/Src/calibration.c \
../Core/Src/flight_control.c \
../Core/Src/gpio.c \
../Core/Src/gps.c \
../Core/Src/i2c.c \
../Core/Src/imu.c \
../Core/Src/main.c \
../Core/Src/rtc.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/u_ubx_protocol.c \
../Core/Src/ultrasonic.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/ahrs.o \
./Core/Src/bmm150.o \
./Core/Src/bmp2.o \
./Core/Src/bmp280.o \
./Core/Src/bridge.o \
./Core/Src/buzzer.o \
./Core/Src/calibration.o \
./Core/Src/flight_control.o \
./Core/Src/gpio.o \
./Core/Src/gps.o \
./Core/Src/i2c.o \
./Core/Src/imu.o \
./Core/Src/main.o \
./Core/Src/rtc.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/u_ubx_protocol.o \
./Core/Src/ultrasonic.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/ahrs.d \
./Core/Src/bmm150.d \
./Core/Src/bmp2.d \
./Core/Src/bmp280.d \
./Core/Src/bridge.d \
./Core/Src/buzzer.d \
./Core/Src/calibration.d \
./Core/Src/flight_control.d \
./Core/Src/gpio.d \
./Core/Src/gps.d \
./Core/Src/i2c.d \
./Core/Src/imu.d \
./Core/Src/main.d \
./Core/Src/rtc.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/u_ubx_protocol.d \
./Core/Src/ultrasonic.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/BBB/repo/stm_drone/drone_f411/F411_FC_STATIC/Inc" -I"D:/BBB/repo/stm_drone/drone_f411/F411_D2/Core" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ahrs.cyclo ./Core/Src/ahrs.d ./Core/Src/ahrs.o ./Core/Src/ahrs.su ./Core/Src/bmm150.cyclo ./Core/Src/bmm150.d ./Core/Src/bmm150.o ./Core/Src/bmm150.su ./Core/Src/bmp2.cyclo ./Core/Src/bmp2.d ./Core/Src/bmp2.o ./Core/Src/bmp2.su ./Core/Src/bmp280.cyclo ./Core/Src/bmp280.d ./Core/Src/bmp280.o ./Core/Src/bmp280.su ./Core/Src/bridge.cyclo ./Core/Src/bridge.d ./Core/Src/bridge.o ./Core/Src/bridge.su ./Core/Src/buzzer.cyclo ./Core/Src/buzzer.d ./Core/Src/buzzer.o ./Core/Src/buzzer.su ./Core/Src/calibration.cyclo ./Core/Src/calibration.d ./Core/Src/calibration.o ./Core/Src/calibration.su ./Core/Src/flight_control.cyclo ./Core/Src/flight_control.d ./Core/Src/flight_control.o ./Core/Src/flight_control.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/gps.cyclo ./Core/Src/gps.d ./Core/Src/gps.o ./Core/Src/gps.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/imu.cyclo ./Core/Src/imu.d ./Core/Src/imu.o ./Core/Src/imu.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/u_ubx_protocol.cyclo ./Core/Src/u_ubx_protocol.d ./Core/Src/u_ubx_protocol.o ./Core/Src/u_ubx_protocol.su ./Core/Src/ultrasonic.cyclo ./Core/Src/ultrasonic.d ./Core/Src/ultrasonic.o ./Core/Src/ultrasonic.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

