SRCS=$(wildcard slam*.cpp)
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs  ibex)
LIBDIR	 := $(shell pkg-config --libdir  ibex) 

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall -frounding-math -ffloat-store -msse2 -mfpmath=sse
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG -Wno-deprecated -frounding-math -ffloat-store -msse2 -mfpmath=sse
endif

all: $(BINS)

data.o : data.cpp data.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< $(LIBS)

% :	%.cpp data.o
	echo $(BINS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(BINS) *.o
	
