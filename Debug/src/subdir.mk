################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DisplayThread.cpp \
../src/FireServer.cpp \
../src/FireThread.cpp \
../src/IntruderThread.cpp \
../src/MainThread.cpp \
../src/ServerThread.cpp \
../src/main.cpp 

OBJS += \
./src/DisplayThread.o \
./src/FireServer.o \
./src/FireThread.o \
./src/IntruderThread.o \
./src/MainThread.o \
./src/ServerThread.o \
./src/main.o 

CPP_DEPS += \
./src/DisplayThread.d \
./src/FireServer.d \
./src/FireThread.d \
./src/IntruderThread.d \
./src/MainThread.d \
./src/ServerThread.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


