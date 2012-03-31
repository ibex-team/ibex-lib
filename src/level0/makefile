
include ../common.mak

OBJS=ibex_Interval.o ibex_IntervalVector.o ibex_IntervalMatrix.o ibex_IntervalMatrixArray.o ibex_Dim.o
TARGET=all

SUBLIB_H:=ibex_$(SUBLIB)_Interval.h
SUBLIB_CPP:=ibex_$(SUBLIB)_Interval.cpp
SUBLIB_DEF:=_IBEX_WITH_`echo $(SUBLIB) | tr '[:lower:]' '[:upper:]'`_
  
$(TARGET):	$(OBJS)
	find *.h -exec install -m 640 {} $(IBEX_INC_DIR)/{} \; ; \
	install -m 640 $(SUBLIB_H) $(IBEX_INC_DIR)/$(SUBLIB_H)
 
ibex_IntervalVector.o : ibex_IntervalVector.cpp ibex_IntervalVector.h ibex_Interval.h $(SUBLIB_CPP) $(SUBLIB_H) 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<

ibex_IntervalMatrix.o : ibex_IntervalMatrix.cpp ibex_IntervalMatrix.h ibex_IntervalVector.h 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<

ibex_IntervalMatrixArray.o : ibex_IntervalMatrixArray.cpp ibex_IntervalMatrixArray.h ibex_IntervalMatrix.h 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<
	
ibex_Interval.o : ibex_Interval.cpp ibex_Interval.h $(SUBLIB_CPP) $(SUBLIB_H) 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<

ibex_Dim.o : ibex_Dim.cpp ibex_Dim.h $(SUBLIB_CPP) $(SUBLIB_H) 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<
		
ibex_Interval.h : update_sub_lib

.PHONY update_sub_lib :
	echo \/\* this file is generated \*\/ > ibex_bias_or_gaol.h ; \
	echo \#define $(SUBLIB_DEF) >> ibex_bias_or_gaol.h 

clean:
	rm -f $(OBJS) ; \
	find *.h -exec rm -f $(IBEX_INC_DIR)/{} \; ; \
	rm -f ibex_bias_or_gaol.h
	