################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/ode/main/dicho-scalable.cpp \
../plugins/ode/main/ibexode-attract-region.cpp \
../plugins/ode/main/optim-scalable.cpp 

OBJS += \
./plugins/ode/main/dicho-scalable.o \
./plugins/ode/main/ibexode-attract-region.o \
./plugins/ode/main/optim-scalable.o 

CPP_DEPS += \
./plugins/ode/main/dicho-scalable.d \
./plugins/ode/main/ibexode-attract-region.d \
./plugins/ode/main/optim-scalable.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/ode/main/%.o: ../plugins/ode/main/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


