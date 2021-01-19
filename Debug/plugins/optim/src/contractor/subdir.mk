################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/optim/src/contractor/ibex_CtcOptimShaving.cpp 

OBJS += \
./plugins/optim/src/contractor/ibex_CtcOptimShaving.o 

CPP_DEPS += \
./plugins/optim/src/contractor/ibex_CtcOptimShaving.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/optim/src/contractor/%.o: ../plugins/optim/src/contractor/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


