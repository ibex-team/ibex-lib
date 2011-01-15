#########################################################################
#
# Lr Makefile
# -----------
#
# Copyright (C) 1992 Dirk Husung
#
# This file is part of PROFIL/BIAS.
#
# PROFIL/BIAS is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.
#
# $Id: Makefile 478 2006-08-09 13:13:30Z keil $
# 
#########################################################################
BASEDIR		= ../..

INSTALLDIR	= lr
LIBRARY		= lr
INCLUDES	= LrDefs.h Lr.h LrIntern.h Li.h DynMem.h
OBJECTS		= LrOp0.o LrOp1.o LrOp2.o LrSc.o LrCvt0.o LrCvt1.o LrCvt2.o \
		  LrIntern.o LrMem.o DynMem.o \
		  LiOp0.o LiOp1.o LiCvt1.o LiCvt2.o \
		  LiMem.o
EXAMPLELIBS	= lr
EXAMPLES	= LrPi LiEuler

DEFINES		= -D__$(ENDIAN)ENDIAN__

include $(BASEDIR)/Host.cfg
include $(BASEDIR)/src/Rules.mak

override CFLAGS	= -g

LrOp0.o:	LrOp0.c Lr.h Portab.h
LrOp1.o:	LrOp1.c Lr.h Portab.h
LrOp2.o:	LrOp2.c Lr.h Portab.h
LrSc.o:		LrSc.c DynMem.h Lr.h Portab.h
LrCvt0.o:	LrCvt0.c Lr.h Portab.h
LrCvt1.o:	LrCvt1.c Lr.h Portab.h
LrCvt2.o:	LrCvt2.c Lr.h Portab.h
LrIntern.o:	LrIntern.c Lr.h Portab.h
LrMem.o:	LrMem.c DynMem.h Lr.h Portab.h
DynMem.o:	DynMem.c DynMem.h Lr.h Portab.h

LiOp0.o:	LiOp0.c Li.h Lr.h Portab.h
LiOp1.o:	LiOp1.c Li.h Lr.h Portab.h
LiCvt1.o:	LiCvt1.c Li.h Lr.h Portab.h
LiCvt2.o:	LiCvt2.c Li.h Lr.h Portab.h
LiMem.o:	LiMem.c DynMem.h Li.h Lr.h Portab.h

Lr.h:		LrDefs.h
Li.h:		Lr.h
