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

IBEX=..

include $(IBEX)/common.mak

LIBS:=$(LIB_IBEX)/libibex.a $(LIB_PROFIL)/libProfil.a $(LIB_PROFIL)/libBias.a $(LIB_PROFIL)/liblr.a $(LIB_PROFIL)/libProfil.a 

INC_JNI=/usr/lib/jni

ifeq ($(CCPLUS),)
  all debug :
	@echo "you must compile Profil/Bias before (cd ..; make)"
else
  ifeq ($(LIBMODE), dynamic)
    LIBS:=$(LIBS:.a=.$(DYNLIBEXT)) 
  endif

  all debug : libquimper.$(DYNLIBEXT) Quimper.class 
endif

Quimper.class : Quimper.java
	javac Quimper.java

libquimper.$(DYNLIBEXT) : Quimper.o $(LIBS)
	$(CCPLUS) $(DYNLIBFLAGS) -o $@ $^

Quimper.o : Quimper.h Quimper.c
	$(CCPLUS) $(CFLAGS) -I$(INC_JNI) -I$(INC_PROFIL) -I$(INC_IBEX) -c -o $@ Quimper.c 

clean : 
	@rm -f *.o *~ *.class *.so

