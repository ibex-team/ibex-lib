################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/sip/src/marendet/strategy/ibex_CellBufferNeighborhood.cpp \
../plugins/sip/src/marendet/strategy/ibex_SIPManifold.cpp \
../plugins/sip/src/marendet/strategy/ibex_SIPOptimizer.cpp \
../plugins/sip/src/marendet/strategy/ibex_SIPSolver.cpp \
../plugins/sip/src/marendet/strategy/ibex_SIPSolverOutputBox.cpp 

OBJS += \
./plugins/sip/src/marendet/strategy/ibex_CellBufferNeighborhood.o \
./plugins/sip/src/marendet/strategy/ibex_SIPManifold.o \
./plugins/sip/src/marendet/strategy/ibex_SIPOptimizer.o \
./plugins/sip/src/marendet/strategy/ibex_SIPSolver.o \
./plugins/sip/src/marendet/strategy/ibex_SIPSolverOutputBox.o 

CPP_DEPS += \
./plugins/sip/src/marendet/strategy/ibex_CellBufferNeighborhood.d \
./plugins/sip/src/marendet/strategy/ibex_SIPManifold.d \
./plugins/sip/src/marendet/strategy/ibex_SIPOptimizer.d \
./plugins/sip/src/marendet/strategy/ibex_SIPSolver.d \
./plugins/sip/src/marendet/strategy/ibex_SIPSolverOutputBox.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/sip/src/marendet/strategy/%.o: ../plugins/sip/src/marendet/strategy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


