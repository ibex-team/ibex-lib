SRCS=$(wildcard *.cpp)
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

include ../src/common.mak
 
#CPPTEST_DIR:=/home/gchabert08/Applications/cpptest
#CPPTEST_INC_DIR:=$(CPPTEST_DIR)/src
#CPPTEST_LIB_DIR:=$(CPPTEST_DIR)
#CPPTEST_INC_DIR_OPT:=-I$(CPPTEST_INC_DIR)
#CPPTEST_LIB_DIR_OPT:=-static -L$(CPPTEST_LIB_DIR)
#CPPTEST_LIB_DIR_OPT:=-static 
CPPTEST_LIB_NAME_OPT:=-lcpptest
 		
INC_DIR_OPT:=$(INC_DIR_OPT) $(CPPTEST_INC_DIR_OPT)
LIB_DIR_OPT:=$(LIB_DIR_OPT) $(CPPTEST_LIB_DIR_OPT)
LIB_NAME_OPT:=$(LIB_NAME_OPT) $(CPPTEST_LIB_NAME_OPT)

$(TARGET):	$(OBJS) $(IBEX_LIB_DIR)/libibex.a
	$(CXX) -o $(TARGET) $(LIB_DIR_OPT) $(OBJS) $(LIB_NAME_OPT) 

utest.o : $(SRCS) $(HEADERS) 

%.o : %.cpp %.h 
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -c  -o  $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
	