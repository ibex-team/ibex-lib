SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags ibex) -O3 -DNDEBUG -Wno-deprecated #-g #-pg 
LIBS	 := $(shell pkg-config --libs   ibex) 

all: $(BINS)

% :	%.cpp
	echo $(BINS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)
	
