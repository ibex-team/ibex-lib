SRCS=$(filter-out vibes.cpp, $(wildcard *.cpp))
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags ibex) 
LIBS	 := $(shell pkg-config --libs  ibex)

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall 
else
CXXFLAGS := $(CXXFLAGS) -O3 -DNDEBUG 
endif

DOC_OUT_TXT=doc-arithmetic.txt doc-modeling.txt doc-separator.txt doc-set.txt 
DOC_SET_OUT=set-sep set-example set-inter set-union set-contract set-interval

all: $(BINS)

% :	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS) $(DOC_OUT_TXT) $(DOC_SET_OUT)
	
