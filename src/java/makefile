include ../common.mak

all: ibex_Java.o Test.class

Test.class : Test.java
	javac Test.java
   
Ibex.class : Ibex.java
	javac Ibex.java
	
ibex_Java.h_ : Ibex.class
	javah -jni Ibex; mv Ibex.h ibex_Java.h_

ibex_Java.o : ibex_Java.cpp ibex_Java.h_
	$(CXX) $(CXXFLAGS) -I$(JAVA_INC) $(INC_DIR_OPT) -c -o $@ $< 

# not only $(OBJS), there may be old .o files
clean:
	rm -f *.o ibex_Java.h_ Ibex.class Test.class 