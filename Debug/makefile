################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tests/subdir.mk
-include src/tools/subdir.mk
-include src/system/subdir.mk
-include src/symbolic/subdir.mk
-include src/strategy/subdir.mk
-include src/solver/subdir.mk
-include src/set/subdir.mk
-include src/predicate/subdir.mk
-include src/parser/subdir.mk
-include src/operators/subdir.mk
-include src/numeric/subdir.mk
-include src/function/subdir.mk
-include src/data/subdir.mk
-include src/contractor/subdir.mk
-include src/combinatorial/subdir.mk
-include src/bisector/subdir.mk
-include src/bin/subdir.mk
-include src/arithmetic/subdir.mk
-include plugins/sip/src/mitsos/subdir.mk
-include plugins/sip/src/marendet/tools/subdir.mk
-include plugins/sip/src/marendet/system/subdir.mk
-include plugins/sip/src/marendet/strategy/subdir.mk
-include plugins/sip/src/marendet/numeric/subdir.mk
-include plugins/sip/src/marendet/loup/subdir.mk
-include plugins/sip/src/marendet/contractors/subdir.mk
-include plugins/sip/src/subdir.mk
-include plugins/sip/main/subdir.mk
-include plugins/param-estim/tests/subdir.mk
-include plugins/param-estim/src/cliquer/subdir.mk
-include plugins/param-estim/src/subdir.mk
-include plugins/param-estim/examples/subdir.mk
-include plugins/optim/tests/subdir.mk
-include plugins/optim/src/strategy/subdir.mk
-include plugins/optim/src/loup/subdir.mk
-include plugins/optim/src/data/subdir.mk
-include plugins/optim/src/contractor/subdir.mk
-include plugins/optim/src/bisector/subdir.mk
-include plugins/optim/main/subdir.mk
-include plugins/optim/subdir.mk
-include plugins/ode/src/subdir.mk
-include plugins/ode/main/subdir.mk
-include plugins/ensta-robotics/tests/subdir.mk
-include plugins/ensta-robotics/src/subdir.mk
-include plugins/continuation/tests/subdir.mk
-include plugins/continuation/src/subdir.mk
-include plugins/continuation/examples/subdir.mk
-include plugins/ampl/tests/subdir.mk
-include plugins/ampl/src/system/subdir.mk
-include plugins/ampl/examples/subdir.mk
-include plugins/affine/tests/subdir.mk
-include plugins/affine/src/numeric/subdir.mk
-include plugins/affine/src/function/subdir.mk
-include plugins/affine/src/arithmetic/subdir.mk
-include plugins/affine/examples/subdir.mk
-include plugins/affine-extended/src/arithmetic/subdir.mk
-include plugins/affine-extended/examples/subdir.mk
-include lp_lib_wrapper/soplex/subdir.mk
-include lp_lib_wrapper/none/subdir.mk
-include lp_lib_wrapper/cplex/subdir.mk
-include interval_lib_wrapper/gaol/subdir.mk
-include interval_lib_wrapper/filib/subdir.mk
-include interval_lib_wrapper/direct/subdir.mk
-include interval_lib_wrapper/bias/subdir.mk
-include examples/slam/subdir.mk
-include examples/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CPP__DEPS)),)
-include $(CPP__DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ibex_2.5

# Tool invocations
ibex_2.5: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "ibex_2.5" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(CPP__DEPS)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) ibex_2.5
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
