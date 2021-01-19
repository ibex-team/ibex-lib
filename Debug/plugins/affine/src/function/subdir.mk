################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/affine/src/function/ibex_AffineEval.cpp 

OBJS += \
./plugins/affine/src/function/ibex_AffineEval.o 

CPP_DEPS += \
./plugins/affine/src/function/ibex_AffineEval.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/affine/src/function/%.o: ../plugins/affine/src/function/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


