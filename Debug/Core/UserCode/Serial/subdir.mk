################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/UserCode/Serial/serial.c 

OBJS += \
./Core/UserCode/Serial/serial.o 

C_DEPS += \
./Core/UserCode/Serial/serial.d 


# Each subdirectory must supply rules for building sources it contributes
Core/UserCode/Serial/%.o Core/UserCode/Serial/%.su Core/UserCode/Serial/%.cyclo: ../Core/UserCode/Serial/%.c Core/UserCode/Serial/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-UserCode-2f-Serial

clean-Core-2f-UserCode-2f-Serial:
	-$(RM) ./Core/UserCode/Serial/serial.cyclo ./Core/UserCode/Serial/serial.d ./Core/UserCode/Serial/serial.o ./Core/UserCode/Serial/serial.su

.PHONY: clean-Core-2f-UserCode-2f-Serial

