SRCS=$(wildcard Test*.cpp) ExFunction.cpp Ponts30.cpp utils.cpp utest.cpp
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs ibex libcpptest)

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall -frounding-math -ffloat-store  -Wno-unknown-pragmas  -Wno-unused-variable
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG -Wno-unused-variable -frounding-math 
endif

$(TARGET):	$(OBJS) 	
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)


utest.o : $(SRCS) $(HEADERS) 

%.o :	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< $(LIBS)

nonreg : nonreg.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
	

