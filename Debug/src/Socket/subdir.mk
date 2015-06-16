################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Socket/ClientSocket.cpp \
../src/Socket/ServerSocket.cpp \
../src/Socket/Socket.cpp 

OBJS += \
./src/Socket/ClientSocket.o \
./src/Socket/ServerSocket.o \
./src/Socket/Socket.o 

CPP_DEPS += \
./src/Socket/ClientSocket.d \
./src/Socket/ServerSocket.d \
./src/Socket/Socket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Socket/%.o: ../src/Socket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


