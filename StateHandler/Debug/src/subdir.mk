################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Counter.cpp \
../src/StateHandler.cpp 

OBJS += \
./src/Counter.o \
./src/StateHandler.o 

CPP_DEPS += \
./src/Counter.d \
./src/StateHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -D__LPC15XX__ -I"/home/tylen/Desktop/Programming/Projects_Metropolia/ESP-Ventilation/StateHandler/inc" -I"/home/tylen/Desktop/Programming/Projects_Metropolia/ESP-Ventilation/lpc_board_nxp_lpcxpresso_1549/inc" -I"/home/tylen/Desktop/Programming/Projects_Metropolia/ESP-Ventilation/lpc_chip_15xx/inc" -I"/home/tylen/Desktop/Programming/Projects_Metropolia/ESP-Ventilation/DigitalIoPin/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


