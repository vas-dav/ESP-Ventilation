################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PressureWrapper.cpp 

CPP_DEPS += \
./src/PressureWrapper.d 

OBJS += \
./src/PressureWrapper.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -D__LPC15XX__ -I"/Users/evgenymeshcheryakov/Documents/Metropolia/3rd/Embedded Systems Programming TX00CI61-3011/ESP-Ventilation/PressureWrapper/inc" -I"/Users/evgenymeshcheryakov/Documents/Metropolia/3rd/Embedded Systems Programming TX00CI61-3011/ESP-Ventilation/lpc_board_nxp_lpcxpresso_1549/inc" -I"/Users/evgenymeshcheryakov/Documents/Metropolia/3rd/Embedded Systems Programming TX00CI61-3011/ESP-Ventilation/lpc_chip_15xx/inc" -I"/Users/evgenymeshcheryakov/Documents/Metropolia/3rd/Embedded Systems Programming TX00CI61-3011/ESP-Ventilation/DigitalIoPin/inc" -I"/Users/evgenymeshcheryakov/Documents/Metropolia/3rd/Embedded Systems Programming TX00CI61-3011/ESP-Ventilation/I2C/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/PressureWrapper.d ./src/PressureWrapper.o

.PHONY: clean-src

