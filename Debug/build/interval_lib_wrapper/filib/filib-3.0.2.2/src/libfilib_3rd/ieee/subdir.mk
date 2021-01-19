################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/ieee/primitive.cpp 

OBJS += \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/ieee/primitive.o 

CPP_DEPS += \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/ieee/primitive.d 


# Each subdirectory must supply rules for building sources it contributes
build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/ieee/%.o: ../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/ieee/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


