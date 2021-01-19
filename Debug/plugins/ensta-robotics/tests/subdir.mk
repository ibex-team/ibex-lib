################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/ensta-robotics/tests/TestCtcPixelMap.cpp \
../plugins/ensta-robotics/tests/TestCtcSegment.cpp \
../plugins/ensta-robotics/tests/TestPixelMap.cpp \
../plugins/ensta-robotics/tests/TestSepPolygon.cpp 

OBJS += \
./plugins/ensta-robotics/tests/TestCtcPixelMap.o \
./plugins/ensta-robotics/tests/TestCtcSegment.o \
./plugins/ensta-robotics/tests/TestPixelMap.o \
./plugins/ensta-robotics/tests/TestSepPolygon.o 

CPP_DEPS += \
./plugins/ensta-robotics/tests/TestCtcPixelMap.d \
./plugins/ensta-robotics/tests/TestCtcSegment.d \
./plugins/ensta-robotics/tests/TestPixelMap.d \
./plugins/ensta-robotics/tests/TestSepPolygon.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/ensta-robotics/tests/%.o: ../plugins/ensta-robotics/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


