SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)

CPPFLAGS := -I../include -I../3rd/gdtoa-1.0 -I../__build__/3rd/gdtoa-1.0 -I../3rd/gaol-3.1.1 -I../3rd/gaol-3.1.1 -I../__build__/3rd/gaol-3.1.1 -I../../soplex-1.7.0/src
CXXFLAGS := -O3 -DNDEBUG -Wno-deprecated #-g #-pg 

LIBS	 := ../lib/libibex.a -lgaol -lgdtoa -lultim -lsoplex -lz
LDFLAGS  := -L../__build__/3rd -L../../soplex-1.7.0/lib

% :	%.cpp ../lib/libibex.a
	echo $(BINS) $(OBJS)
	$(CXX) -static $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINS)
	
