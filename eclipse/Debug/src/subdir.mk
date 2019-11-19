################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/prenos/remc2/remc2/remc2.cpp 

OBJS += \
./src/remc2.o 

CPP_DEPS += \
./src/remc2.d 


# Each subdirectory must supply rules for building sources it contributes
src/remc2.o: C:/prenos/remc2/remc2/remc2.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


