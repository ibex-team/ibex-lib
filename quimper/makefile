#--------------------------------------------------------------------------------
#                    Makefile for QUIMPER
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

IBEX=..

include $(IBEX)/common.mak

ifeq ($(OS), Windows_NT)
  GTK_GL_EXT=gtkglext-win32-1.0
  LD_OPENGL=-lglut32 -lopengl32 -lglu32 -lgdi32 -lwinmm
  EXEC=qPave.exe qSolve.exe qTraj.exe 2Dviewer.exe
  WIN_FLAGS_I= -mms-bitfields
  WIN_FLAGS = $(WIN_FLAGS_I) -fnative-struct
else
  GTK_GL_EXT=gtkglext-x11-1.0
  LD_OPENGL=-lGLU -lglut -lX11
  EXEC=qPave qSolve qTraj 2Dviewer
endif

LIB_GL=/usr/X11R6/lib
LD_GL=-L$(LIB_GL) -lGL -lGLU -lglut -lX11 

LD_PLIB=-lplibpw -lplibfnt -lplibsg -lplibul

INC_GTK_GL=/usr/include/gtkglext-1.0/

LD_GTK_GL=-L/usr/lib/ -l$(GTK_GL_EXT) -lgtkgl -lgdkglext-x11-1.0


ifeq ($(CCPLUS),)
  all debug :
	@echo "you must compile Profil/Bias before (cd ..; make)"
else
  all debug : $(EXEC)
endif

paver_ibex.o : paver_ibex.cpp paver_ibex.h ibex.cpp ibex.h paver_gui.h gui.h
	$(CCPLUS) $(CFLAGS) -c -I$(INC_PROFIL) -I$(INC_IBEX) -o $@ $<

solver_ibex.o : solver_ibex.cpp solver_ibex.h ibex.cpp ibex.h gui.h
	$(CCPLUS) $(CFLAGS) -c -I$(INC_PROFIL) -I$(INC_IBEX) -o $@ $<

traj_ibex.o : traj_ibex.cpp traj_ibex.h ibex.cpp ibex.h gui.h
	$(CCPLUS) $(CFLAGS) -c -I$(INC_PROFIL) -I$(INC_IBEX) -o $@ $<

viewer.o : viewer.cpp viewer.h
	$(CCPLUS) -c -o $@ $< 

paver_gui.o : paver_gui.c paver_gui.h gui.c gui.h
	$(CC) $(CFLAGS) -c $(WIN_FLAGS_I) `pkg-config --cflags-only-I gtkglext-1.0 gtk+-2.0` -o $@ $< 

solver_gui.o : solver_gui.c gui.c gui.h
	$(CC) $(CFLAGS) -c $(WIN_FLAGS_I) `pkg-config --cflags-only-I gtkglext-1.0 gtk+-2.0` -o $@ $< 

traj_gui.o : traj_gui.c gui.c gui.h
	$(CC) $(CFLAGS) -c $(WIN_FLAGS_I) `pkg-config --cflags-only-I gtkglext-1.0 gtk+-2.0` -o $@ $< 

qPave qPave.exe: paver_ibex.o paver_gui.o viewer.o
	$(CC) $(CFLAGS) -o $@ $^ $(WIN_FLAGS) `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 $(GTK_GL_EXT)` $(LD_IBEX) $(LD_PROFIL) $(LD_OPENGL) -lstdc++

qSolve qSolve.exe: solver_ibex.o solver_gui.o
	$(CC) $(CFLAGS) -o $@ $^ $(WIN_FLAGS) `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 $(GTK_GL_EXT)` $(LD_IBEX) $(LD_PROFIL) -lstdc++

qTraj qTraj.exe: traj_ibex.o traj_gui.o viewer.o
	$(CC) $(CFLAGS) -o $@ $^ $(WIN_FLAGS) `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 $(GTK_GL_EXT)` $(LD_IBEX) $(LD_PROFIL) $(LD_OPENGL) -lstdc++

2Dviewer 2Dviewer.exe: 2Dviewer.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LD_PROFIL) $(LD_OPENGL) -lstdc++

clean :
	@rm -f *.o $(EXEC) *~ *.dat 


