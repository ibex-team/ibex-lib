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

MAIN_SRC=$(wildcard *.cpp)
PARS_DIR=parser
OPTIM_DIR=optim
PARS_SRC=$(wildcard $(PARS_DIR)/*.cpp) 
OPTIM_SRC=$(wildcard $(OPTIM_DIR)/*.cpp) 
OBJS=$(MAIN_SRC:.cpp=.o) $(PARS_DIR)/lexer.o $(PARS_DIR)/parser.o $(PARS_SRC:.cpp=.o) $(OPTIM_SRC:.cpp=.o)


ifeq ($(CCPLUS),)
  all debug :
	@echo "you must compile Profil/Bias before (cd ..; make)"
else
  ifeq ($(LIBMODE), dynamic)
    all debug : $(LIB_IBEX)/libibex.$(DYNLIBEXT)
	cd contrib; $(MAKE) $(MAKECMDGOALS)
  else
    all debug : $(LIB_IBEX)/libibex.a
	cd contrib; $(MAKE) $(MAKECMDGOALS)
  endif
endif


%.o : %.cpp %.h
	$(CCPLUS) $(CFLAGS) -I$(INC_PROFIL) -I. -I./parser -c -o $@ $<

$(LIB_IBEX)/libibex.a : $(OBJS)
	ar rcs $@ $(OBJS); \
	mkdir -p $(INC_IBEX)/$(OPTIM_DIR); \
	find *.h $(OPTIM_DIR)/*.h -exec install -m 640 {} $(INC_IBEX)/{} \;

$(LIB_IBEX)/libibex.$(DYNLIBEXT) : $(OBJS)
	$(CCPLUS) $(DYNLIBFLAGS) -o $@ $^
	find *.h $(OPTIM_DIR)/*.h -exec install -m 640 {} $(INC_IBEX)/{} \;

$(PARS_DIR)/parser.o $(PARS_DIR)/parser.h: $(PARS_DIR)/parser.yacc 
	bison --name-prefix=ibex --defines $(PARS_DIR)/parser.yacc --report=all --file-prefix=parser; \
	mv parser.tab.hacc $(PARS_DIR)/parser.h; \
	mv parser.tab.cacc $(PARS_DIR)/parser.c; \
	$(CCPLUS) $(CFLAGS) -I$(INC_PROFIL) -I$(IBEX)/src $(PARS_DIR)/parser.c -c -o $(PARS_DIR)/parser.o
 
$(PARS_DIR)/lexer.c : $(PARS_DIR)/lexer.lex $(PARS_DIR)/ParserExpr.h
	flex -Pibex -o$(PARS_DIR)/lexer.c $(PARS_DIR)/lexer.lex

$(PARS_DIR)/lexer.o : $(PARS_DIR)/lexer.c $(PARS_DIR)/parser.h $(PARS_DIR)/ParserExpr.h
	$(CCPLUS) $(CFLAGS) -I$(INC_PROFIL) -I$(IBEX)/src $(PARS_DIR)/lexer.c -c -o $@

clean : 
	@rm -f *.o *~ $(LIB_IBEX)/libibex.* $(INC_IBEX)/*.h \
	$(OPTIM_DIR)/*.o $(OPTIM_DIR)/*~ \
	$(PARS_DIR)/*.o $(PARS_DIR)/*~ *.output $(PARS_DIR)/parser.h $(PARS_DIR)/parser.c $(PARS_DIR)/lexer.h $(PARS_DIR)/lexer.c; \
	cd contrib; $(MAKE) clean
