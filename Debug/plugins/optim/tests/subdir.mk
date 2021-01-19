################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/optim/tests/TestCellHeap.cpp \
../plugins/optim/tests/TestCtcPolytopeHull.cpp \
../plugins/optim/tests/TestOptimizer.cpp \
../plugins/optim/tests/TestUnconstrainedLocalSearch.cpp \
../plugins/optim/tests/nonreg.cpp 

OBJS += \
./plugins/optim/tests/TestCellHeap.o \
./plugins/optim/tests/TestCtcPolytopeHull.o \
./plugins/optim/tests/TestOptimizer.o \
./plugins/optim/tests/TestUnconstrainedLocalSearch.o \
./plugins/optim/tests/nonreg.o 

CPP_DEPS += \
./plugins/optim/tests/TestCellHeap.d \
./plugins/optim/tests/TestCtcPolytopeHull.d \
./plugins/optim/tests/TestOptimizer.d \
./plugins/optim/tests/TestUnconstrainedLocalSearch.d \
./plugins/optim/tests/nonreg.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/optim/tests/%.o: ../plugins/optim/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


