SRCS=$(wildcard *.cpp)
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

CXXFLAGS := $(shell pkg-config --cflags ibex)  -O3 -DNDEBUG -Wno-deprecated   #-g # -pg 
LIBS	 :=  -static $(shell pkg-config --libs  ibex) -lcpptest
LIBDIR	 := $(shell pkg-config --libdir  ibex) 

$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)


utest.o : $(SRCS) $(HEADERS) 

%.o :	%.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -c -o $@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
	

