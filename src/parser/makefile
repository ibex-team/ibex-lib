SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak
  
$(TARGET):	$(OBJS)
 
%.o : %.cpp lexer.o parser.o
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. -c  -o  $@ $<

lexer.o : lexer.c parser.tab.hh
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. lexer.c -c -o $@

parser.o parser.tab.hh: parser.yc 
	bison --name-prefix=ibex --defines parser.yc --report=all --file-prefix=parser; \
	mv parser.tab.hc parser.tab.hh; \
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. parser.tab.cc -c -o parser.o
 
lexer.c : lexer.l ibex_Scope.h
	flex -Pibex -olexer.c lexer.l

clean:
	rm -f *~ *.o *~ *.output parser.tab.hh parser.tab.cc lexer.h lexer.c; \
