
CONTRIB_NAME=mohc

######################################

IBEX=../../..
CONTRIB=$(IBEX)/src/contrib

include $(IBEX)/common.mak

SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)
LIB=ibex$(CONTRIB_NAME)

ifeq ($(LIBMODE), dynamic)
  all debug : $(LIB_IBEX)/lib$(LIB).$(DYNLIBEXT)

else
  all debug : $(LIB_IBEX)/lib$(LIB).a

endif

$(LIB_IBEX)/lib$(LIB).a : $(OBJS)
	ar rcs $@ $(OBJS); \
	mkdir -p $(INC_IBEX)/$(CONTRIB_NAME); \
	find *.h -exec install -m 640 {} $(INC_IBEX)/$(CONTRIB_NAME)/{} \;

$(LIB_IBEX)/lib$(LIB).$(DYNLIBEXT) : $(OBJS)
	$(CCPLUS) $(DYNLIBFLAGS) -o $@ $^
	mkdir -p $(INC_IBEX)/$(CONTRIB_NAME); \
	find *.h -exec install -m 640 {} $(INC_IBEX)/$(CONTRIB_NAME)/{} \;

%.o : %.cpp %.h
	$(CCPLUS) $(CFLAGS) -I$(INC_PROFIL) -I$(IBEX)/src -I$(CONTRIB) -c -o $@ $<

clean : 
	@rm -f *.o *~ $(LIB_IBEX)/lib$(LIB).a $(LIB_IBEX)/lib$(LIB).$(DYNLIBEXT); \
	rm -Rf $(INC_IBEX)/$(CONTRIB_NAME)