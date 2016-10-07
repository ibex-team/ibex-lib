
IBEX_TEST_DIR=../../../tests/
SRCS=$(wildcard Test*.cpp) utest.cpp utils.cpp
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs ibex cppunit)

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall -frounding-math -ffloat-store -Wno-unknown-pragmas -Wno-unused-variable
#CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -pedantic -frounding-math -ffloat-store  -Waddress -Warray-bounds  -Wc++11-compat  -Wchar-subscripts   -Wenum-compare    -Wcomment  -Wformat    -Wmain -Wmaybe-uninitialized  -Wmissing-braces -Wnonnull     -Wparentheses    -Wreorder  -Wreturn-type   -Wsequence-point  -Wsign-compare  -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunused-function -Wunused-label    -Wunused-value    -Wunused-variable -Wvolatile-register-var 
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG -Wno-unused-variable -frounding-math 
endif

$(TARGET):	$(OBJS) 	
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)

utest.o : $(IBEX_TEST_DIR)/utest.cpp $(IBEX_TEST_DIR)/utest.h 
	$(CXX) $(CPPFLAGS) -I$(IBEX_TEST_DIR) $(CXXFLAGS) -c -o $@ $< $(LIBS)
	
utils.o : $(IBEX_TEST_DIR)/utils.cpp $(IBEX_TEST_DIR)/utils.h 
	$(CXX) $(CPPFLAGS) -I$(IBEX_TEST_DIR) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o :	%.cpp
	$(CXX) $(CPPFLAGS) -I$(IBEX_TEST_DIR) $(CXXFLAGS) -c -o $@ $< $(LIBS)

nonreg : nonreg.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET) nonreg
	

