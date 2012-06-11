
ifeq ($(MAKECMDGOALS),dynamic)
  export LIBMODE=dynamic
else
  export LIBMODE=static
endif

all debug dynamic: 
	cd BiasProfil; ./Configure; $(MAKE); $(MAKE) install; \
	cd ../src; $(MAKE); \
	cd ../wrapC; $(MAKE); \
	cd ../quimper; $(MAKE); \
        cd ../soplex-1.5.0; $(MAKE); \
	#cd ../jni; \$(MAKE); 
	cd .. 

clean:
	cd BiasProfil; $(MAKE) clean; rm -f lib/*.a lib/*.so; \
	cd ../src; $(MAKE) clean; \
	cd ../wrapC; $(MAKE) clean; \
	cd ../quimper; $(MAKE) clean; \
        cd ../soplex-1.5.0; $(MAKE) clean; \
	#cd ../jni; $(MAKE) clean; \
	cd ..; rm -f *~;
