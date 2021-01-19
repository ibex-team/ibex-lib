################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/sip/main/ibexsip.cpp \
../plugins/sip/main/optimizer.cpp \
../plugins/sip/main/solver.cpp 

OBJS += \
./plugins/sip/main/ibexsip.o \
./plugins/sip/main/optimizer.o \
./plugins/sip/main/solver.o 

CPP_DEPS += \
./plugins/sip/main/ibexsip.d \
./plugins/sip/main/optimizer.d \
./plugins/sip/main/solver.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/sip/main/%.o: ../plugins/sip/main/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


