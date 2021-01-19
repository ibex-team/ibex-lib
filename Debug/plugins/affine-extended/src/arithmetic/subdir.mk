################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/affine-extended/src/arithmetic/ibex_Affine2_No.cpp \
../plugins/affine-extended/src/arithmetic/ibex_Affine2_fAF1.cpp \
../plugins/affine-extended/src/arithmetic/ibex_Affine2_iAF.cpp \
../plugins/affine-extended/src/arithmetic/ibex_Affine2_sAF.cpp 

OBJS += \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_No.o \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_fAF1.o \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_iAF.o \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_sAF.o 

CPP_DEPS += \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_No.d \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_fAF1.d \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_iAF.d \
./plugins/affine-extended/src/arithmetic/ibex_Affine2_sAF.d 


# Each subdirectory must supply rules for building sources it contributes
plugins/affine-extended/src/arithmetic/%.o: ../plugins/affine-extended/src/arithmetic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


