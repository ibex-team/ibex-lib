#--------------------------------------------------------------------------------
#                    Makefile
#                   ----------
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


#-----------------------------------------------------------
# Set the BIAS/PROFIL libs PATH
PROFIL=$(IBEX)/BiasProfil
INC_PROFIL=$(PROFIL)/include
BASEDIR=$(PROFIL)
-include $(PROFIL)/Host.cfg
LIB_PROFIL=$(PROFIL)/lib
LD_PROFIL=-L$(LIB_PROFIL) -lProfil -lBias -llr #-lstdc++ -lm #-lProfilPackages 
#--------------------------------------------------

ifeq ($(MAKECMDGOALS), debug)
  CFLAGS:=-O0 -Wall -Wreturn-type -g #-pg
else
  CFLAGS:=$(CFLAGS) -Wno-deprecated
endif

#-----------------------------------------------------------
# Set the IBEX libs PATH
INC_IBEX=$(IBEX)/include
LIB_IBEX=$(IBEX)/lib
LD_IBEX=-L$(LIB_IBEX) -libex
#--------------------------------------------------

