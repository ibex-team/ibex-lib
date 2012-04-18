SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak

SUBLIB_H:=ibex_$(SUBLIB)_Interval.h_
SUBLIB_CPP:=ibex_$(SUBLIB)_Interval.cpp
SUBLIB_DEF:=_IBEX_WITH_`echo $(SUBLIB) | tr '[:lower:]' '[:upper:]'`_
  
%.o : %.cpp update_sub_lib
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c  -o  $@ $<
	
	
$(TARGET):	$(OBJS)
	find *.h -exec install -m 640 {} $(IBEX_INC_DIR)/{} \; ; \
	install -m 640 $(SUBLIB_H) $(IBEX_INC_DIR)/$(SUBLIB_H)
 
ibex_Interval.h : update_sub_lib

.PHONY update_sub_lib :
	echo \/\* this file is generated \*\/ > ibex_bias_or_gaol.h ; \
	echo \#define $(SUBLIB_DEF) >> ibex_bias_or_gaol.h 

clean:
	rm -f $(OBJS) ; \
	find *.h -exec rm -f $(IBEX_INC_DIR)/{} \; ; \
	rm -f ibex_bias_or_gaol.h
	