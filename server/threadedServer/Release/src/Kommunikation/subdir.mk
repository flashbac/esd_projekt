################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Kommunikation/Kommunikation.cpp \
../src/Kommunikation/KommunikationsProtokoll.cpp 

OBJS += \
./src/Kommunikation/Kommunikation.o \
./src/Kommunikation/KommunikationsProtokoll.o 

CPP_DEPS += \
./src/Kommunikation/Kommunikation.d \
./src/Kommunikation/KommunikationsProtokoll.d 


# Each subdirectory must supply rules for building sources it contributes
src/Kommunikation/%.o: ../src/Kommunikation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


