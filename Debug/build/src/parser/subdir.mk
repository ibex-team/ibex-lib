################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../build/src/parser/lexer.lex.cc \
../build/src/parser/parser.tab.cc 

CC_DEPS += \
./build/src/parser/lexer.lex.d \
./build/src/parser/parser.tab.d 

OBJS += \
./build/src/parser/lexer.lex.o \
./build/src/parser/parser.tab.o 


# Each subdirectory must supply rules for building sources it contributes
build/src/parser/%.o: ../build/src/parser/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/jninin/Logiciel/CPLEX/cplex/include -I/home/jninin/Logiciel/filib/include -I/home/jninin/Logiciel/soplex-1.7.2/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


