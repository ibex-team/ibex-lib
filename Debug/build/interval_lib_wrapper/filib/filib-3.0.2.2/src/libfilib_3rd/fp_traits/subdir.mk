################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_double_setup.cpp \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_float_setup.cpp \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_double_setup.cpp \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_float_setup.cpp \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_sse_const.cpp \
../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_x87_const.cpp 

OBJS += \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_double_setup.o \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_float_setup.o \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_double_setup.o \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_float_setup.o \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_sse_const.o \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_x87_const.o 

CPP_DEPS += \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_double_setup.d \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_base_float_setup.d \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_double_setup.d \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_float_setup.d \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_sse_const.d \
./build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/fp_traits_x87_const.d 


# Each subdirectory must supply rules for building sources it contributes
build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/%.o: ../build/interval_lib_wrapper/filib/filib-3.0.2.2/src/libfilib_3rd/fp_traits/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


