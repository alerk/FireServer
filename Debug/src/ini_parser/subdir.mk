################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ini_parser/dictionary.cpp \
../src/ini_parser/iniparser.cpp 

OBJS += \
./src/ini_parser/dictionary.o \
./src/ini_parser/iniparser.o 

CPP_DEPS += \
./src/ini_parser/dictionary.d \
./src/ini_parser/iniparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/ini_parser/%.o: ../src/ini_parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


