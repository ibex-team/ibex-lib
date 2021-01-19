################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/operators/ibex_atanhc.cpp \
../src/operators/ibex_atanhccc.cpp \
../src/operators/ibex_crossproduct.cpp \
../src/operators/ibex_sinc.cpp \
../src/operators/ibex_trace.cpp 

OBJS += \
./src/operators/ibex_atanhc.o \
./src/operators/ibex_atanhccc.o \
./src/operators/ibex_crossproduct.o \
./src/operators/ibex_sinc.o \
./src/operators/ibex_trace.o 

CPP_DEPS += \
./src/operators/ibex_atanhc.d \
./src/operators/ibex_atanhccc.d \
./src/operators/ibex_crossproduct.d \
./src/operators/ibex_sinc.d \
./src/operators/ibex_trace.d 


# Each subdirectory must supply rules for building sources it contributes
src/operators/%.o: ../src/operators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


