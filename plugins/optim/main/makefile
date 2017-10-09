SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs  ibex)

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CXXFLAGS := $(CXXFLAGS) -std=c++11 -I../../../examples

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG
endif

EX_WITH_VIBES="doc-contractor"
VIBES_MSG="INFO: The example $@ uses VIBes to plot data, you need to launch VIBes-viewer before running this test"

all: $(BINS)

% :	%.cpp
	@case $(EX_WITH_VIBES) in *$@*) echo "$(VIBES_MSG)" ;; esac
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)
	
