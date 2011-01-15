#########################################################################
#
# BIAS Makefile (Watcom wmake)
# ----------------------------
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
# $Id: Makefile.wat 478 2006-08-09 13:13:30Z keil $
#
#########################################################################

BASEDIR		= ..\..
ARCHDIR		= $(BASEDIR)\config\x86-Windows95-compat-watcom

INSTALLDIR	= BIAS
LIBRARY		= Bias
INCLUDES	= Bias0.h Bias1.h Bias2.h BiasF.h BiasType.h
OBJECTS		= Bias0.obj Bias1.obj Bias2.obj BiasF.obj BiasRnd.obj

DEFINES		= -D__$(ENDIAN)ENDIAN__ -D__$(COMPARE)COMPARE__

!include $(ARCHDIR)/Host.cfg

all:		$(LIBRARY).lib

$(LIBRARY).lib:	$(OBJECTS)
	$(AR) $(LIBRARY) +Bias0.obj +Bias1.obj +Bias2.obj +BiasF.obj +BiasRnd.obj

Bias0.obj:	Bias0.c Bias0.h BiasInt.h $(ARCHDIR)\BiasRnd.h
Bias1.obj:	Bias1.c Bias1.h BiasInt.h $(ARCHDIR)\BiasRnd.h
Bias2.obj:	Bias2.c Bias2.h BiasInt.h $(ARCHDIR)\BiasRnd.h
BiasF.obj:	BiasF.c BiasF.h BiasInt.h $(ARCHDIR)\BiasRnd.h
BiasRnd.obj:	$(ARCHDIR)\BiasRnd.c $(ARCHDIR)\BiasRnd.h

Bias1.h:	Bias0.h
Bias2.h:	Bias1.h
BiasF.h:	Bias0.h

.c.obj:
	$(CC) $(CFLAGS) $(DEFINES) -I. -I$(ARCHDIR) -o $@ -c $<
