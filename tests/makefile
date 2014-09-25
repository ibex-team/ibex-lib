SRCS=$(wildcard *.cpp)
HEADERS=$(SRCS:.cpp=.h)
OBJS=$(SRCS:.cpp=.o)
TARGET=utest

CXXFLAGS := $(shell pkg-config --cflags /home/jninin/Documents/WORK/DEV/IBEX/GIT/OUT_debug/share/pkgconfig/ibex.pc) 
LIBS	 := $(shell pkg-config --libs /home/jninin/Documents/WORK/DEV/IBEX/GIT/OUT_debug/share/pkgconfig/ibex.pc libcpptest)

ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -pedantic -frounding-math -ffloat-store  -Waddress -Warray-bounds  -Wc++11-compat  -Wchar-subscripts   -Wenum-compare    -Wcomment  -Wformat    -Wmain -Wmaybe-uninitialized  -Wmissing-braces -Wnonnull     -Wparentheses    -Wreorder  -Wreturn-type   -Wsequence-point  -Wsign-compare  -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunused-function -Wunused-label    -Wunused-value    -Wunused-variable -Wvolatile-register-var 
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
	

