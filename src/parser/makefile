SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=all

include ../common.mak
  
$(TARGET):	$(OBJS)
 
%.o : %.cpp lexer.o parser.o
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. -c  -o  $@ $<

lexer.o : lexer.c parser.h
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. lexer.c -c -o $@

parser.o parser.h: parser.yacc 
	bison --name-prefix=ibex --defines parser.yacc --report=all --file-prefix=parser; \
	mv parser.tab.hacc parser.h; \
	mv parser.tab.cacc parser.c; \
	$(CXX) $(CXXFLAGS) $(INC_DIR_OPT) -I. parser.c -c -o parser.o
 
lexer.c : lexer.lex ibex_Scope.h
	flex -Pibex -olexer.c lexer.lex

clean:
	rm -f $(OBJS) *~ *.o *~ *.output parser.h parser.c lexer.h lexer.c; \