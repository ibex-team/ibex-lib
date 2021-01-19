################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/param-estim/tests/TestQInter2.cpp 

OBJS += \
./plugins/param-estim/tests/TestQInter2.o 

CPP_DEPS += \
./plugins/param-estim/tests/TestQInter2.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/param-estim/tests/%.o: ../plugins/param-estim/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


