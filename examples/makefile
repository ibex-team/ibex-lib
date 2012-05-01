SRCS=$(wildcard *.cpp)
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=robust_estim

include ../src/common.mak

% :	%.o $(IBEX_LIB_DIR)/libibex.a
	$(CXX) -o $(TARGET) $(LIB_DIR_OPT) $< $(LIB_NAME_OPT)  

%.o : %.cpp 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c -o  $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
	