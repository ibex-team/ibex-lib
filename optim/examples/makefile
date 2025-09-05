SRCS= $(wildcard *.cpp) 

BINS=$(SRCS:.cpp=)

 
CXXFLAGS := $(shell pkg-config --cflags ibex) $(shell pkg-config --cflags ibex-affine) $(shell pkg-config --cflags ibex-ampl) 
LIBS	 := $(shell pkg-config --libs ibex) $(shell pkg-config --libs ibex-affine) $(shell pkg-config --libs  ibex-ampl) 

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CXXFLAGS := $(CXXFLAGS) -std=c++11 

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG
endif


all: $(BINS)

% :	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)

