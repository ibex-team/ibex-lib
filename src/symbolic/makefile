SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak
  
$(TARGET):	$(OBJS)
 
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c  -o  $@ $<

clean:
	rm -f $(OBJS);