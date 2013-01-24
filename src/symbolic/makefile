SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak
  
$(TARGET):	$(OBJS)
 
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c  -o  $@ $<

# not only $(OBJS), there may be old .o files
clean:
	rm -f *.o 