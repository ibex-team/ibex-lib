SRCS=$(wildcard *.cpp)
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs ibex libcpptest)

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall -frounding-math  
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG -Wno-unused-variable -frounding-math 
endif

$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)


utest.o : $(SRCS) $(HEADERS) 

%.o :	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
	

