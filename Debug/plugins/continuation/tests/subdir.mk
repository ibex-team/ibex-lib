################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/continuation/tests/TestCellDiff.cpp \
../plugins/continuation/tests/TestCont.cpp \
../plugins/continuation/tests/TestParametricProof.cpp 

OBJS += \
./plugins/continuation/tests/TestCellDiff.o \
./plugins/continuation/tests/TestCont.o \
./plugins/continuation/tests/TestParametricProof.o 

CPP_DEPS += \
./plugins/continuation/tests/TestCellDiff.d \
./plugins/continuation/tests/TestCont.d \
./plugins/continuation/tests/TestParametricProof.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/continuation/tests/%.o: ../plugins/continuation/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


