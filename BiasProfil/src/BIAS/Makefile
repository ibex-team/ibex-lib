#########################################################################
#
# BIAS Makefile (GNU make)
# ------------------------
#
# Copyright (C) 1998 Olaf Knueppel
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

INSTALLDIR	= BIAS
LIBRARY		= Bias
INCLUDES	= Bias0.h Bias1.h Bias2.h BiasF.h BiasType.h

EXAMPLELIBS	= Bias
EXAMPLES	= BiasTest BiasTimes

DEFINES		= -D__$(ENDIAN)ENDIAN__ -D__$(COMPARE)COMPARE__
MOREINCLUDES	= $(ARCHDIR)

include $(BASEDIR)/Host.cfg

ifeq ($(ARCH), z_fenv)
  OBJECTS	= Bias0.o Bias1.o Bias2.o BiasF.o BiasRnd.o
else
  OBJECTS	= Bias0.o Bias1.o Bias2.o BiasF.o BiasRnd.o fpRound.o
endif

include $(BASEDIR)/src/Rules.mak


Bias0.o:	Bias0.c Bias0.h BiasInt.h $(ARCHDIR)/BiasRnd.h
Bias1.o:	Bias1.c Bias1.h BiasInt.h $(ARCHDIR)/BiasRnd.h
Bias2.o:	Bias2.c Bias2.h BiasInt.h $(ARCHDIR)/BiasRnd.h
BiasF.o:	BiasF.c BiasF.h BiasInt.h $(ARCHDIR)/BiasRnd.h
BiasRnd.o:	$(ARCHDIR)/BiasRnd.c $(ARCHDIR)/BiasRnd.h
fpRound.o:	$(ARCHDIR)/fpRound.s

Bias1.h:	Bias0.h
Bias2.h:	Bias1.h
BiasF.h:	Bias0.h

%.o:			$(ARCHDIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) $(addprefix -I,$(INCLUDE)) -o $@ -c $<

%.o:			$(ARCHDIR)/%.s
	$(AS) -o $@ $<

