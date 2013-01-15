SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

CXXFLAGS := $(shell pkg-config --cflags /home/nininjo/Documents/WORK/PROJET/IBEX/share/pkgconfig/ibex.pc)  -O3 -DNDEBUG -Wno-deprecated -frounding-math -g # -pg 
LIBS	 := $(shell pkg-config --libs   /home/nininjo/Documents/WORK/PROJET/IBEX/share/pkgconfig/ibex.pc) 

all: $(BINS)

% :	%.cpp
	echo $(BINS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)
	
