################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/UserCode/PID/pid.c 

OBJS += \
./Core/UserCode/PID/pid.o 

C_DEPS += \
./Core/UserCode/PID/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/UserCode/PID/%.o Core/UserCode/PID/%.su Core/UserCode/PID/%.cyclo: ../Core/UserCode/PID/%.c Core/UserCode/PID/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-UserCode-2f-PID

clean-Core-2f-UserCode-2f-PID:
	-$(RM) ./Core/UserCode/PID/pid.cyclo ./Core/UserCode/PID/pid.d ./Core/UserCode/PID/pid.o ./Core/UserCode/PID/pid.su

.PHONY: clean-Core-2f-UserCode-2f-PID

