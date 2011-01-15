#########################################################################
#
# PROFIL/BIAS Makefile Rules (GNU make)
# -------------------------------------
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
# $Id: Rules.mak 478 2006-08-09 13:13:30Z keil $
#
#########################################################################
.SECONDARY : $(OBJECTS)

LIBDIR		= $(BASEDIR)/lib
INCLUDEDIR	= $(BASEDIR)/include
ARCHDIR		= $(BASEDIR)/config/$(ARCH)

SRCDIR		= $(BASEDIR)/src
INCLUDE		= . $(SRCDIR) $(SRCDIR)/Base $(SRCDIR)/Packages $(MOREINCLUDES)
LINKEXAMPLELIBS	= $(addprefix -l,$(EXAMPLELIBS))

ifeq ($(LIBMODE), dynamic)
LIBRARYBASE	= lib$(notdir $(LIBRARY)).$(DYNLIBEXT)
else
LIBRARYBASE	= lib$(notdir $(LIBRARY)).a
endif

LIBRARYNAME	= $(dir $(LIBRARY))$(LIBRARYBASE)

ifneq ($(strip $(LIBRARY)),)
all:			$(LIBRARYNAME)
else
all:
endif

ifeq ($(LIBMODE), dynamic)
$(LIBRARYNAME):		$(OBJECTS)
	$(CCPLUS) $(DYNLIBFLAGS) -o $@ $^
else
$(LIBRARYNAME):		$(LIBRARYNAME)($(OBJECTS))
	$(RANLIB) $@

$(LIBRARYNAME)(%.o):	%.o
	$(AR) $@ $<
endif

ifneq ($(strip $(IAS)),)
%.o:			%.c
	$(CC) $(CFLAGS) $(DEFINES) $(addprefix -I,$(INCLUDE)) $(IASFLAGS) -o $(basename $@).is -c $<
	$(IAS) -o $(basename $@).isc $(basename $@).is
	$(AS) -o $@ $(basename $@).isc
	$(RM) $(basename $@).is $(basename $@).isc

%.o:			%.C
	$(CCPLUS) $(CPLUSFLAGS) $(DEFINES) $(addprefix -I,$(INCLUDE)) $(IASFLAGS) -o $(basename $@).is -c $<
	$(IAS) -o $(basename $@).isc $(basename $@).is
	$(AS) -o $@ $(basename $@).isc
	$(RM) $(basename $@).is $(basename $@).isc
else
%.o:			%.c
	$(CC) $(CFLAGS) $(DEFINES) $(addprefix -I,$(INCLUDE)) -o $@ -c $<

%.o:			%.C
	$(CCPLUS) $(CPLUSFLAGS) $(DEFINES) $(addprefix -I,$(INCLUDE)) -o $@ -c $<
endif

%.o:			%.s
	$(AS) -o $@ $<

%$(TESTEXT):		%.o
	$(LINK) $(LINKFLAGS) -o $@ $< -L$(LIBDIR) $(LINKEXAMPLELIBS) $(SYSLIBS)

examples:		$(addsuffix $(TESTEXT),$(EXAMPLES))

clean:
	-@$(RM) $(LIBRARYNAME) $(OBJECTS) $(addsuffix $(TESTEXT),$(EXAMPLES))

install:		all
ifneq ($(strip $(LIBRARY)),)
	-@test -d $(LIBDIR)                   || mkdir $(LIBDIR)
endif
	-@test -d $(INCLUDEDIR)               || mkdir $(INCLUDEDIR)
	-@test -d $(INCLUDEDIR)/$(INSTALLDIR) || mkdir $(INCLUDEDIR)/$(INSTALLDIR)
ifneq ($(strip $(LIBRARY)),)
	$(INSTALL) $(LIBRARYNAME) $(LIBDIR)
ifeq ($(LIBMODE), dynamic)
	#export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(LIBDIR)
else
	$(RANLIB) $(LIBDIR)/$(LIBRARYBASE)
endif
endif
	$(INSTALL) $(INCLUDES) $(INCLUDEDIR)/$(INSTALLDIR)

uninstall:
	-@$(RM) $(LIBDIR)/$(LIBRARYBASE)
	-@$(RM) $(addprefix $(INCLUDEDIR)/$(INSTALLDIR)/,$(INCLUDES))
	-@$(RMDIR) $(INCLUDEDIR)/$(INSTALLDIR)
	-@$(RMDIR) $(INCLUDEDIR)
	-@$(RMDIR) $(LIBDIR)

register:
	-@mkdir RCS
	@set allow_null_glob_expansion; \
	 for file in *.[cCSh] Makefile; do \
	  echo Registering $$file; \
	  ci -u -t- $$file; \
	 done

#
# Local Variables:
# mode: Makefile
# End:
#
