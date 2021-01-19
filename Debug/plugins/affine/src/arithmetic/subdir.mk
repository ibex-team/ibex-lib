################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/affine/src/arithmetic/ibex_Affine2_fAF2.cpp \
../plugins/affine/src/arithmetic/ibex_Affine3_fAFFullI.cpp 

OBJS += \
./plugins/affine/src/arithmetic/ibex_Affine2_fAF2.o \
./plugins/affine/src/arithmetic/ibex_Affine3_fAFFullI.o 

CPP_DEPS += \
./plugins/affine/src/arithmetic/ibex_Affine2_fAF2.d \
./plugins/affine/src/arithmetic/ibex_Affine3_fAFFullI.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/affine/src/arithmetic/%.o: ../plugins/affine/src/arithmetic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


