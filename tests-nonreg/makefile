#--------------------------------------------------------------------------------
#                    Makefile for examples
#                   -----------------------
#
# Copyright (C) 2007 Gilles Chabert
# 
# This file is part of IBEX.
#
# IBEX is free software; you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation; either 
# version 2 of the License, or (at your option) any later version.
#
# IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with IBEX; 
# if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
# Boston, MA 02110-1301, USA 
#--------------------------------------------------------------------------------

CC = g++

FLAGS=-D__I386__ -O -Wno-deprecated  -g #-pg 

SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXE=nonreg

IBEX=..

#-----------------------------------------------------------
# Set the BIAS/PROFIL libs PATH
PROFIL=$(IBEX)/BiasProfil
INC_PROFIL=$(PROFIL)/include
LIB_PROFIL=$(PROFIL)/lib
LD_PROFIL=-L$(LIB_PROFIL) -lProfilPackages -lProfil -lBias -llr #-lstdc++ -lm
#--------------------------------------------------

#-----------------------------------------------------------
# Set the IBEX libs PATH
INC_IBEX=$(IBEX)/include
LIB_IBEX=$(IBEX)/lib
LD_IBEX=-L$(LIB_IBEX) -libex
#--------------------------------------------------

$(EXE) : $(OBJ) ../lib/libibex.a
	$(CC) $(FLAGS) -I$(INC_PROFIL) -I$(INC_IBEX) -o $@ $^ $(LD_IBEX) $(LD_PROFIL) -pthread

%.o : %.cc
	$(CC) $(FLAGS) -I$(INC_PROFIL) -I$(INC_IBEX) -c -o $@ $^

clean : 
	@rm -f $(OBJ) $(EXE) *~