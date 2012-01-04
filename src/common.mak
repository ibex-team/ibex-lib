
# chose the interval library
#===================================================
#!warning: do not insert white space after the name! 
SUBLIB=gaol
#SUBLIB=bias
#===================================================

# where are Ibex, Bias/Profil and Gaol installed?
#!warning: gdtoa must also be installed in the root directory of Gaol
#===================================================
IBEX_DIR:=/home/gchabert/workspace/ibex
BIAS_DIR:=/home/gchabert/Profil-2.0.8
GAOL_DIR:=/home/gchabert/gaol-3.1.1
#===================================================

CXXFLAGS :=	-O2 -g -Wall -fmessage-length=0

IBEX_INC_DIR:=$(IBEX_DIR)/include
IBEX_INC_DIR_OPT:=-I$(IBEX_INC_DIR)
IBEX_LIB_DIR=$(IBEX_DIR)/lib
IBEX_LIB_DIR_OPT=-L$(IBEX_LIB_DIR)
IBEX_LIB_NAME_OPT=-libex

BIAS_INC_DIR :=$(BIAS_DIR)/include
BIAS_INC_DIR_OPT:=-I$(BIAS_INC_DIR)
BIAS_LIB_DIR:=$(BIAS_DIR)/lib
BIAS_LIB_DIR_OPT:=-L$(BIAS_LIB_DIR) 
BIAS_LIB_NAME_OPT:=-lProfil -lBias -llr

GAOL_INC_DIR  := $(GAOL_DIR)/gaol
GAOL_LIB_DIR  := $(GAOL_DIR)/gaol/.libs
GDTOA_DIR  := $(GAOL_DIR)/gdtoa
GDTOA_LIB_DIR := $(GDTOA_DIR)/.libs
GAOL_INC_DIR_OPT := -I$(GAOL_INC_DIR)
GAOL_LIB_DIR_OPT := -L$(GAOL_LIB_DIR) -L$(GDTOA_LIB_DIR)
GAOL_LIB_NAME_OPT := -lgaol -lgdtoa -lm -lultim 

INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(GAOL_INC_DIR_OPT) $(BIAS_INC_DIR_OPT)
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(GAOL_LIB_DIR_OPT) $(BIAS_LIB_DIR_OPT)
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(GAOL_LIB_NAME_OPT) $(BIAS_LIB_NAME_OPT) 

