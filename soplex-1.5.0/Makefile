#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#
#*                                                                           *#
#*                  This file is part of the class library                   *#
#*       SoPlex --- the Sequential object-oriented simPlex.                  *#
#*                                                                           *#
#*    Copyright (C) 1996      Roland Wunderling                              *#
#*                  1996-2010 Konrad-Zuse-Zentrum                            *#
#*                            fuer Informationstechnik Berlin                *#
#*                                                                           *#
#*  SoPlex is distributed under the terms of the ZIB Academic Licence.       *#
#*                                                                           *#
#*  You should have received a copy of the ZIB Academic License              *#
#*  along with SoPlex; see the file COPYING. If not email to soplex@zib.de.  *#
#*                                                                           *#
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#
# $Id: Makefile,v 1.111 2010/10/01 18:42:03 bzfgleix Exp $		      

#@file    Makefile
#@brief   SoPlex Makefile
#@author  Thorsten Koch

.PHONY:		all depend clean distclean lint doc check test

VERSION		:=	1.5.0

ARCH		:=	$(shell uname -m | \
			sed \
			-e 's/sun../sparc/' \
			-e 's/i.86/x86/' \
			-e 's/i86pc/x86/' \
			-e 's/[0-9]86/x86/' \
			-e 's/amd64/x86_64/' \
			-e 's/IP../mips/' \
			-e 's/9000..../hppa/' \
			-e 's/Power\ Macintosh/ppc/' \
			-e 's/00........../pwr4/' )
OSTYPE		:=	$(shell uname -s | tr '[:upper:]' '[:lower:]' | \
			sed \
			-e 's/cygwin.*/cygwin/' \
			-e 's/irix../irix/' \
			-e 's/windows.*/windows/' \
			-e 's/mingw.*/mingw/')
HOSTNAME	:=	$(shell uname -n | tr '[:upper:]' '[:lower:]')

VERBOSE		=	false
OPT		=	opt
LIBEXT		=	a
EXEEXTENSION	=	
TEST		=	quick
ALGO		=	1 2 3 4
LIMIT		=	#

ZLIB		=	false

COMP		=	gnu
CXX		=	g++
CXX_c		=	-c # the trailing space is important
CXX_o		=	-o # the trailing space is important
LINKCXX		=	g++
LINKCXX_L	=	-L
LINKCXX_l	=	-l
LINKCXX_o	=	-o # the trailing space is important
LINKLIBSUFFIX	=
DCXX		=	g++
LINT		=	flexelint
AR		=	ar
AR_o		=
RANLIB		=	ranlib
DOXY		=	doxygen
VALGRIND	=	valgrind
LN_s		=	ln -s

LIBBUILD	=	$(AR)
LIBBUILD_o	=	$(AR_o)
LIBBUILDFLAGS	=       $(ARFLAGS)

CPPFLAGS	=	-Isrc
CXXFLAGS	=	
BINOFLAGS	=	
LIBOFLAGS	=	
LDFLAGS		=	$(LINKCC_l)m$(LINKLIBSUFFIX)
ARFLAGS		=	cr
DFLAGS		=	-MM
VFLAGS		=	--tool=memcheck --leak-check=yes --show-reachable=yes #--gen-suppressions=yes

SRCDIR		=	src
BINDIR		=	bin
LIBDIR		=	lib
NAME		=	soplex
LIBOBJ		= 	changesoplex.o didxset.o \
			dsvector.o dvector.o enter.o factor.o \
			forest.o idxset.o leave.o lpcolset.o lprowset.o \
			lprow.o message.o mpsinput.o nameset.o \
			slufactor.o solve.o soplex.o \
			spxbasis.o spxbounds.o spxchangebasis.o \
			spxequilisc.o spxdantzigpr.o spxdefaultrt.o \
			spxdefines.o spxdesc.o spxdevexpr.o \
			spxfastrt.o spxfileio.o spxgeometsc.o \
			spxharrisrt.o spxhybridpr.o spxid.o spxio.o \
			spxlp.o spxlpfread.o spxmainsm.o spxmpsread.o \
			spxmpswrite.o spxlpfwrite.o \
			spxout.o spxparmultpr.o spxquality.o \
			spxscaler.o spxshift.o spxsolver.o spxsolve.o \
			spxstarter.o spxsteeppr.o spxsumst.o spxvecs.o \
			spxvectorst.o spxweightpr.o spxweightst.o spxwritestate.o \
			ssvector.o svector.o svset.o timer.o \
			tracemethod.o unitvector.o update.o updatevector.o \
			vector.o vsolve.o \
			gzstream.o
BINOBJ		=	soplexmain.o
EXAMPLEOBJ	=	simpleexample.o
REPOSIT		=	# template repository, explicitly empty  #spxproof.o 

BASE		=	$(OSTYPE).$(ARCH).$(COMP).$(OPT)

LASTSETTINGS	=	$(OBJDIR)/make.lastsettings

#------------------------------------------------------------------------------
#--- NOTHING TO CHANGE FROM HERE ON -------------------------------------------
#------------------------------------------------------------------------------

GCCWARN		=	-Wall -W -Wpointer-arith -Wno-unknown-pragmas \
			-Wcast-align -Wwrite-strings -Wconversion \
			-Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder \
			-Woverloaded-virtual -Wsign-promo -Wsynth -Wundef \
			-Wcast-qual -Wold-style-cast -Wshadow 
#			-Weffc++ -Wredundant-decls    
# gcc 2.xx -Wmissing-declarations -Wbad-function-cast 

#GCCWARN =
#-----------------------------------------------------------------------------
include make/make.$(BASE)
-include make/local/make.$(HOSTNAME)
-include make/local/make.$(HOSTNAME).$(COMP)
-include make/local/make.$(HOSTNAME).$(COMP).$(OPT)
#-----------------------------------------------------------------------------

CXXFLAGS	+=	$(USRCXXFLAGS)
LDFLAGS		+=	$(USRLDFLAGS)
ARFLAGS		+=	$(USRARFLAGS)
DFLAGS		+=	$(USRDFLAGS)

#-----------------------------------------------------------------------------

BINNAME		=	$(NAME)-$(VERSION).$(BASE)
EXAMPLENAME	=	simpleexample.$(BASE)
LIBNAME		=	$(NAME)-$(VERSION).$(BASE)
BINFILE		=	$(BINDIR)/$(BINNAME)$(EXEEXTENSION)
EXAMPLEFILE	=	$(BINDIR)/$(EXAMPLENAME)
LIBFILE		=	$(LIBDIR)/lib$(LIBNAME).$(LIBEXT)
LIBSHORTLINK	=	$(LIBDIR)/lib$(NAME).$(LIBEXT)
LIBLINK		=	$(LIBDIR)/lib$(NAME).$(BASE).$(LIBEXT)
BINLINK		=	$(BINDIR)/$(NAME).$(BASE)$(EXEEXTENSION)
BINSHORTLINK	=	$(BINDIR)/$(NAME)$(EXEEXTENSION)
DEPEND		=	src/depend

# potential valgrind suppression file name
VSUPPNAME	= 	$(OSTYPE).$(ARCH).$(COMP).supp

OBJDIR		=	obj/O.$(BASE)
BINOBJDIR	=	$(OBJDIR)/bin
LIBOBJDIR	=	$(OBJDIR)/lib
BINOBJFILES	=	$(addprefix $(BINOBJDIR)/,$(BINOBJ))
EXAMPLEOBJFILES	=	$(addprefix $(BINOBJDIR)/,$(EXAMPLEOBJ))
LIBOBJFILES	=	$(addprefix $(LIBOBJDIR)/,$(LIBOBJ))
BINSRC		=	$(addprefix $(SRCDIR)/,$(BINOBJ:.o=.cpp))
EXAMPLESRC	=	$(addprefix $(SRCDIR)/,$(EXAMPLEOBJ:.o=.cpp))
LIBSRC		=	$(addprefix $(SRCDIR)/,$(LIBOBJ:.o=.cpp))

ZLIBDEP		:=	$(SRCDIR)/depend.zlib
ZLIBSRC		:=	$(shell cat $(ZLIBDEP))
ifeq ($(ZLIB_LDFLAGS),)
ZLIB		=	false
endif
ifeq ($(ZLIB),true)
CPPFLAGS	+=	-DWITH_ZLIB $(ZLIB_FLAGS)
LDFLAGS		+=	$(ZLIB_LDFLAGS)
endif


ifeq ($(VERBOSE),false)
.SILENT:	$(LIBLINK) $(LIBSHORTLINK) $(BINLINK) $(BINSHORTLINK) $(BINFILE) $(LIBFILE) $(BINOBJFILES) $(LIBOBJFILES)
endif

all:		$(LIBFILE) $(BINFILE) $(LIBLINK) $(LIBSHORTLINK) $(BINLINK) $(BINSHORTLINK)
simpleexample:	$(LIBFILE) $(EXAMPLEFILE) $(LIBLINK) $(LIBSHORTLINK)

$(LIBLINK) $(LIBSHORTLINK):	$(LIBFILE)
		@rm -f $@
		cd $(dir $@) && $(LN_s) $(notdir $(LIBFILE)) $(notdir $@)

$(BINLINK) $(BINSHORTLINK):	$(BINFILE)
		@rm -f $@
		cd $(dir $@) && $(LN_s) $(notdir $(BINFILE)) $(notdir $@)

$(BINFILE):	$(BINDIR) $(BINOBJDIR) $(LIBFILE) $(BINOBJFILES)
		@echo "-> linking $@"
		$(LINKCXX) $(BINOBJFILES) \
		$(LINKCXX_L)$(LIBDIR) $(LINKCXX_l)$(LIBNAME)$(LINKLIBSUFFIX) $(LDFLAGS) $(LINKCXX_o)$@

$(EXAMPLEFILE):	$(BINDIR) $(EXAMPLEOBJDIR) $(LIBFILE) $(EXAMPLEOBJFILES)
		@echo "-> linking $@"
		$(LINKCXX) $(EXAMPLEOBJFILES) \
		$(LINKCXX_L)$(LIBDIR) $(LINKCXX_l)$(LIBNAME)$(LINKLIBSUFFIX) $(LDFLAGS) $(LINKCXX_o)$@

$(LIBFILE):	$(LIBDIR) $(LIBOBJDIR) touchexternal $(LIBOBJFILES)
		@echo "-> generating library $@"
		-rm -f $(LIBFILE)
		$(LIBBUILD) $(LIBBUILDFLAGS) $(LIBBUILD_o)$@ $(LIBOBJFILES) $(REPOSIT)
ifneq ($(RANLIB),)
		$(RANLIB) $@
endif


lint:		$(BINSRC) $(LIBSRC)
		$(LINT) lint/$(NAME).lnt -os\(lint.out\) \
		$(CPPFLAGS) -UNDEBUG $^

doc:		
		cd doc; $(DOXY) $(NAME).dxy

all:		$(BINFILE)

test:		check

check:		#$(BINFILE)
		cd check; ./check.sh $(TEST).test ../$(BINFILE) '$(ALGO)' $(LIMIT)

valgrind-check:	$(BINFILE)
		cd check; \
		./valgrind.sh $(TEST).test ../$(BINFILE) '$(ALGO)' '$(LIMIT)' \
		"$(VALGRIND) $(VFLAGS)" $(VSUPPNAME)

memory_exception_test: $(BINFILE)
		cd check; \
		./exception.sh $(TEST).test ../$(BINFILE) '$(ALGO)' '$(LIMIT)' \
		"$(VALGRIND) $(VFLAGS)" $(VSUPPNAME)

clean:
		-rm -rf $(OBJDIR)/* $(BINFILE) $(EXAMPLEFILE) $(LIBFILE) $(LIBLINK) $(BINLINK) $(BINSHORTLINK)

distclean:	clean
		-rm -rf obj/* $(LIBDIR)/lib$(NAME).* $(BINDIR)/$(NAME).* 

vimtags:
		-ctags -o TAGS src/*.cpp src/*.h

etags:
		-ctags -e -o TAGS src/*.cpp src/*.h

$(OBJDIR):	
		@-mkdir -p $(OBJDIR)

$(BINOBJDIR):	$(OBJDIR)
		@-mkdir -p $(BINOBJDIR)

$(LIBOBJDIR):	$(OBJDIR)
		@-mkdir -p $(LIBOBJDIR)

$(BINDIR):
		@-mkdir -p $(BINDIR)

$(LIBDIR):
		@-mkdir -p $(LIBDIR)

depend:
		$(SHELL) -ec '$(DCXX) $(DFLAGS) $(CPPFLAGS) \
		$(BINSRC:.o=.cpp) \
		| sed '\''s|^\([0-9A-Za-z]\{1,\}\)\.o|$$\(BINOBJDIR\)/\1.o|g'\'' \
		>$(DEPEND)'
		$(SHELL) -ec '$(DCXX) $(DFLAGS) $(CPPFLAGS) \
		$(EXAMPLESRC:.o=.cpp) \
		| sed '\''s|^\([0-9A-Za-z]\{1,\}\)\.o|$$\(BINOBJDIR\)/\1.o|g'\'' \
		>>$(DEPEND)'
		$(SHELL) -ec '$(DCXX) $(DFLAGS) $(CPPFLAGS) \
		$(LIBSRC:.o=.cpp) \
		| sed '\''s|^\([0-9A-Za-z]\{1,\}\)\.o|$$\(LIBOBJDIR\)/\1.o|g'\'' \
		>>$(DEPEND)'
		@echo `grep -l "WITH_ZLIB" $(SRCDIR)/*` >$(ZLIBDEP)

-include	$(DEPEND)

$(BINOBJDIR)/%.o:	$(SRCDIR)/%.cpp
		@echo "-> compiling $@"
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(BINOFLAGS) $(CXX_c)$< $(CXX_o)$@

$(LIBOBJDIR)/%.o:	$(SRCDIR)/%.cpp
		@echo "-> compiling $@"
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBOFLAGS) $(CXX_c)$< $(CXX_o)$@


-include $(LASTSETTINGS)

.PHONY: touchexternal
touchexternal:	$(ZLIBDEP)
ifneq ($(ZLIB),$(LAST_ZLIB))
		@-touch $(ZLIBSRC)
endif
		@-rm -f $(LASTSETTINGS)
		@echo "LAST_ZLIB=$(ZLIB)" >> $(LASTSETTINGS)


# --- EOF ---------------------------------------------------------------------
