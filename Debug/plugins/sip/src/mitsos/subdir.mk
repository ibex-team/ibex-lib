################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/sip/src/mitsos/ibex_BD_Factory.cpp \
../plugins/sip/src/mitsos/ibex_LLP_Factory.cpp \
../plugins/sip/src/mitsos/ibex_MitsosSIP.cpp 

OBJS += \
./plugins/sip/src/mitsos/ibex_BD_Factory.o \
./plugins/sip/src/mitsos/ibex_LLP_Factory.o \
./plugins/sip/src/mitsos/ibex_MitsosSIP.o 

CPP_DEPS += \
./plugins/sip/src/mitsos/ibex_BD_Factory.d \
./plugins/sip/src/mitsos/ibex_LLP_Factory.d \
./plugins/sip/src/mitsos/ibex_MitsosSIP.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/sip/src/mitsos/%.o: ../plugins/sip/src/mitsos/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


