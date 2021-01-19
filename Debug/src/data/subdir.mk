################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data/ibex_Cov.cpp \
../src/data/ibex_CovIBUList.cpp \
../src/data/ibex_CovIUList.cpp \
../src/data/ibex_CovList.cpp \
../src/data/ibex_CovManifold.cpp \
../src/data/ibex_CovSolverData.cpp 

OBJS += \
./src/data/ibex_Cov.o \
./src/data/ibex_CovIBUList.o \
./src/data/ibex_CovIUList.o \
./src/data/ibex_CovList.o \
./src/data/ibex_CovManifold.o \
./src/data/ibex_CovSolverData.o 

CPP_DEPS += \
./src/data/ibex_Cov.d \
./src/data/ibex_CovIBUList.d \
./src/data/ibex_CovIUList.d \
./src/data/ibex_CovList.d \
./src/data/ibex_CovManifold.d \
./src/data/ibex_CovSolverData.d 


# Each subdirectory must supply rules for building sources it contributes
src/data/%.o: ../src/data/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


