SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 :=  -static $(shell pkg-config --libs  ibex) -lcpptest
LIBDIR	 := $(shell pkg-config --libdir  ibex) 

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall -frounding-math  
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG -Wno-deprecated -frounding-math 
endif

all: $(BINS)

% :	%.cpp
	echo $(BINS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)
	
