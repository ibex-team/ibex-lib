
# chose the interval library
#===================================================
#!warning: do not insert white space after the name! 
#SUBLIB=gaol
#SUBLIB=bias
SUBLIB=filib
#===================================================

# where are Ibex, Bias/Profil and Gaol installed?
#!warning: gdtoa must also be installed in the root directory of Gaol
# JAVA_INC must contain jni.h
#===================================================
#IBEX_DIR:=/home/gchabert08/Workspace-C++/Ibex
#BIAS_DIR:=/home/gchabert08/Applications/Profil-2.0.8
#GAOL_DIR:=/home/gchabert08/Applications/Gaol-3.1.1
#SIMPLEX_DIR:=/home/gchabert08/Applications/soplex-1.5.0
#JAVA_INC=/usr/lib/jvm/java-7-openjdk-i386/include     

IBEX_DIR:=/home/nininjo/Documents/WORK/PROJET/IBEX
BIAS_DIR:=/home/nininjo/Logiciel/Profil-2.0.8
GAOL_DIR:=/home/nininjo/Logiciel/gaol-3.1.1
FILIB_DIR:=/home/nininjo/Logiciel/filib
SIMPLEX_DIR:=/home/nininjo/Logiciel/soplex-1.7.0
JAVA_INC=/usr/lib/jvm/java-6-openjdk-amd64/include
#===================================================

# To compile in debug mode: "make DEBUG=yes"

ifeq ($(DEBUG), yes)
CXXFLAGS:=-O0 -g -pg -Wall -Wno-deprecated -Wno-unknown-pragmas -fmessage-length=0 -frounding-math  
else
CXXFLAGS:=-O3 -DNDEBUG -Wno-deprecated -frounding-math #-g #-pg 
endif

RELEASE=2.0

IBEX_INC_DIR:=$(IBEX_DIR)/include/ibex
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


FILIB_INC_DIR :=$(FILIB_DIR)/include
FILIB_INC_DIR_OPT:=-I$(FILIB_INC_DIR)
FILIB_LIB_DIR:=$(FILIB_DIR)/lib
FILIB_LIB_DIR_OPT:=-L$(FILIB_LIB_DIR) 
FILIB_LIB_NAME_OPT:=-frounding-math -lprim

GAOL_INC_DIR  := $(GAOL_DIR)/include
GAOL_LIB_DIR  := $(GAOL_DIR)/lib
GDTOA_DIR  := $(GAOL_DIR)
GDTOA_LIB_DIR := $(GDTOA_DIR)
ULTIM_LIB_DIR := $(ULTIM_DIR)/lib
GAOL_INC_DIR_OPT := -I$(GAOL_INC_DIR)
GAOL_LIB_DIR_OPT := -L$(GAOL_LIB_DIR) -L$(GDTOA_LIB_DIR) -L$(ULTIM_LIB_DIR)
GAOL_LIB_NAME_OPT := -lgaol -lgdtoa -lm -lultim 

SIMPLEX_INC_DIR:=$(SIMPLEX_DIR)/src
SIMPLEX_INC_DIR_OPT:=-I$(SIMPLEX_INC_DIR)
SIMPLEX_LIB_DIR=$(SIMPLEX_DIR)/lib
SIMPLEX_LIB_DIR_OPT=-L$(SIMPLEX_LIB_DIR)
#SIMPLEX_LIB_NAME_OPT=-lsoplex-1.7.0.linux.x86_64.gnu.opt
SIMPLEX_LIB_NAME_OPT=-lsoplex

ifeq ($(SUBLIB), gaol)
INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(GAOL_INC_DIR_OPT) $(SIMPLEX_INC_DIR_OPT) 
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(GAOL_LIB_DIR_OPT) $(SIMPLEX_LIB_DIR_OPT) 
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(GAOL_LIB_NAME_OPT) $(SIMPLEX_LIB_NAME_OPT) 
else
ifeq ($(SUBLIB), bias)
INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(BIAS_INC_DIR_OPT) $(SIMPLEX_INC_DIR_OPT)
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(BIAS_LIB_DIR_OPT) $(SIMPLEX_LIB_DIR_OPT)
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(BIAS_LIB_NAME_OPT) $(SIMPLEX_LIB_NAME_OPT)
else
INC_DIR_OPT := $(IBEX_INC_DIR_OPT) $(FILIB_INC_DIR_OPT) $(SIMPLEX_INC_DIR_OPT)
LIB_DIR_OPT := $(IBEX_LIB_DIR_OPT) $(FILIB_LIB_DIR_OPT) $(SIMPLEX_LIB_DIR_OPT)
LIB_NAME_OPT := $(IBEX_LIB_NAME_OPT) $(FILIB_LIB_NAME_OPT) $(SIMPLEX_LIB_NAME_OPT)
endif
endif
