#########################################################################
#
# PROFIL/BIAS Main Makefile (GNU make)
# ------------------------------------
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
# $Id: Makefile 560 2008-10-14 04:56:19Z keil $
#
#########################################################################

all install uninstall examples:		Host.cfg
	@cd src; $(MAKE) $@

clean:
	@cd src; $(MAKE) $@
	@-rm -f Host.cfg

Host.cfg:
	@Configure

TARNAME	= Profil-2.0.6
TMPTAR	= /tmp/$(TARNAME)
TARFILE	= $(TARNAME).tgz

distribution:
	@-rm -Rf $(TMPTAR)
	@mkdir $(TMPTAR)
	@-rm -f $(TARFILE)
	@echo Copying files to $(TMPTAR)
	@find . \
		-name .svn -prune -o \
		-name include -prune -o \
		-name lib -prune -o \
		-name \*~ -o \
		-name \*.tar -o \
		-name \*.tgz -o \
		-name \*.o -o \
		-name \*.a -o \
		-name \*.exe -o \
		-path ./Host.cfg -o \
		-type d -o -print | \
	tar -cSpf - -T - | ( cd $(TMPTAR); tar -xSpf - )
	@echo Building compressed $(TARFILE)
	@tar -czhf $(TARFILE) -C $(TMPTAR)/.. $(TARNAME)
	@-rm -Rf $(TMPTAR)
