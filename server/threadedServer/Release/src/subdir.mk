################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/Helper.cpp \
../src/OpenCVWarpper.cpp \
../src/UDPClient.cpp \
../src/UDPProtkoll.cpp \
../src/main.cpp 

OBJS += \
./src/Client.o \
./src/Helper.o \
./src/OpenCVWarpper.o \
./src/UDPClient.o \
./src/UDPProtkoll.o \
./src/main.o 

CPP_DEPS += \
./src/Client.d \
./src/Helper.d \
./src/OpenCVWarpper.d \
./src/UDPClient.d \
./src/UDPProtkoll.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


