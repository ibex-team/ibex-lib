
# chose the interval library
#===================================================
#!warning: do not insert white space after the name! 
#SUBLIB=gaol
SUBLIB=bias
#===================================================

# where are Ibex, Bias/Profil and Gaol installed?
#!warning: gdtoa must also be installed in the root directory of Gaol
#===================================================
IBEX_DIR:=/home/gchabert08/Workspace-C++/Ibex
BIAS_DIR:=/home/gchabert08/Applications/Profil-2.0.8
GAOL_DIR:=/home/gchabert08/Applications/Gaol-3.1.1
#===================================================

#CXXFLAGS := -g -pg -Wall -Wno-deprecated -fmessage-length=0
CXXFLAGS :=	-O3 -DNDEBUG #-g -pg 

IBEX_INC_DIR:=$(IBEX_DIR)/include
IBEX_SRC_DIR:=$(IBEX_DIR)/src
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
GAOL_LIB_DIR  := $(GAOL_DIR)
GDTOA_DIR  := $(GAOL_DIR)/gdtoa
GDTOA_LIB_DIR := $(GDTOA_DIR)
GAOL_INC_DIR_OPT := -I$(GAOL_INC_DIR)
GAOL_LIB_DIR_OPT := -L$(GAOL_LIB_DIR) -L$(GDTOA_LIB_DIR)
GAOL_LIB_NAME_OPT := -lgaol -lgdtoa -lm -lultim 

ifeq ($(SUBLIB), gaol)
INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(GAOL_INC_DIR_OPT)
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(GAOL_LIB_DIR_OPT)
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(GAOL_LIB_NAME_OPT)
else
INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(BIAS_INC_DIR_OPT)
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(BIAS_LIB_DIR_OPT)
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(BIAS_LIB_NAME_OPT)
endif