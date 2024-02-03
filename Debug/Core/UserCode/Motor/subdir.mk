################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/UserCode/Motor/motor.c 

OBJS += \
./Core/UserCode/Motor/motor.o 

C_DEPS += \
./Core/UserCode/Motor/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/UserCode/Motor/%.o Core/UserCode/Motor/%.su Core/UserCode/Motor/%.cyclo: ../Core/UserCode/Motor/%.c Core/UserCode/Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-UserCode-2f-Motor

clean-Core-2f-UserCode-2f-Motor:
	-$(RM) ./Core/UserCode/Motor/motor.cyclo ./Core/UserCode/Motor/motor.d ./Core/UserCode/Motor/motor.o ./Core/UserCode/Motor/motor.su

.PHONY: clean-Core-2f-UserCode-2f-Motor

