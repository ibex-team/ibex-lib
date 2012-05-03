SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak
  
$(TARGET):	$(OBJS)
	find *.h -exec install -m 640 {} $(IBEX_INC_DIR)/{} \;
 
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c  -o  $@ $<

clean:
	rm -f $(OBJS) ; \
	find *.h -exec rm -f $(IBEX_INC_DIR)/{} \;