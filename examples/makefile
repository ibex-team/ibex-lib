SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

include ../src/common.mak

% :	%.cpp $(IBEX_LIB_DIR)/libibex.a
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -o $@ $< $(LIB_DIR_OPT) -static $(LIB_NAME_OPT)  

clean:
	rm -f $(OBJS) $(BINS)
	