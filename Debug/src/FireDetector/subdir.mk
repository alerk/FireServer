################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FireDetector/CommonFunctions.cpp \
../src/FireDetector/FireDetector.cpp \
../src/FireDetector/STEPIII_TemporalWaveletAnalysis.cpp \
../src/FireDetector/STEPII_DetectionFireColoredPixels.cpp \
../src/FireDetector/STEPIV_SpatialWaveletAnalysis.cpp \
../src/FireDetector/STEPI_MovingRegionDetection.cpp 

OBJS += \
./src/FireDetector/CommonFunctions.o \
./src/FireDetector/FireDetector.o \
./src/FireDetector/STEPIII_TemporalWaveletAnalysis.o \
./src/FireDetector/STEPII_DetectionFireColoredPixels.o \
./src/FireDetector/STEPIV_SpatialWaveletAnalysis.o \
./src/FireDetector/STEPI_MovingRegionDetection.o 

CPP_DEPS += \
./src/FireDetector/CommonFunctions.d \
./src/FireDetector/FireDetector.d \
./src/FireDetector/STEPIII_TemporalWaveletAnalysis.d \
./src/FireDetector/STEPII_DetectionFireColoredPixels.d \
./src/FireDetector/STEPIV_SpatialWaveletAnalysis.d \
./src/FireDetector/STEPI_MovingRegionDetection.d 


# Each subdirectory must supply rules for building sources it contributes
src/FireDetector/%.o: ../src/FireDetector/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


