include common.mak

KERNEL=$(shell uname -s)
STATIC_TARGET:=$(IBEX_LIB_DIR)/libibex.a

ifeq ($(KERNEL), Linux)
DYNAMIC_TARGET:=$(IBEX_LIB_DIR)/libibex.so
else
DYNAMIC_TARGET:=$(IBEX_LIB_DIR)/libibex.dylib
endif

ARITH:=arithmetic
SUBDIRS:=tools $(ARITH) numeric symbolic function combinatorial contractor bisector strategy parser java
SUBLIB_DEF:=_IBEX_WITH_`echo $(SUBLIB) | tr '[:lower:]' '[:upper:]'`_
DISTRIB=ibex-$(RELEASE)-$(KERNEL)
 
# warning: don't define OBJS with ":=" because the
# object files may not exist yet
OBJS  =$(wildcard $(IBEX_SRC_DIR)/tools/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/arithmetic/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/numeric/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/symbolic/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/function/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/combinatorial/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/contractor/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/bisector/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/strategy/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/parser/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/java/*.o)

# The dynamic library encompasses all the dependencies (in a single library 
# that can be directly uploaded, in particular from Choco).
# So we determine here which are the actual dependencies.
ifeq ($(SUBLIB), gaol)
DYN_LIB_DEP=dyn_ibex_gaol
else
DYN_LIB_DEP=dyn_ibex_bias  
endif

all : $(STATIC_TARGET) $(DYNAMIC_TARGET)

$(STATIC_TARGET) : headers $(SUBDIRS)
	ar rcs $@ $(OBJS);
     
headers :
	echo \/\* this file is generated \*\/ > $(ARITH)/ibex_bias_or_gaol.h ; \
	echo \#define $(SUBLIB_DEF) >> $(ARITH)/ibex_bias_or_gaol.h; \
	install -m 640 $(ARITH)/ibex_$(SUBLIB)_Interval.h_ $(IBEX_INC_DIR)/ibex_$(SUBLIB)_Interval.h_; \
	echo \/\* this file is generated \*\/ > $(IBEX_INC_DIR)/ibex.h ; \
	find $(IBEX_SRC_DIR) -name "*.h" | while read LINE; do \
	f=`basename $${LINE}`; echo "#include \""$$f"\"" >> $(IBEX_INC_DIR)/ibex.h; \
	install -m 640 $${LINE} $(IBEX_INC_DIR)/$$f; done

## to remove parser:
##	find $(IBEX_SRC_DIR) -path "$(IBEX_SRC_DIR)/parser*" -prune -o -name "*.h" | grep "\\.h" | while read LINE; do \


$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@ 

# Create temporarily all the .o objects (including Profil/Bias) on the "lib" subdirectory
# in order to create the dynamic library.
# Note: Vector.o (of Bias) is in conflict with soplex, and useless, so it is removed as 
# its dependencies (Matrix, etc.) before uncompressing libsoplex.a.
dyn_ibex_bias: $(STATIC_TARGET)
	cd $(IBEX_LIB_DIR);  ar -x libibex.a; \
	ar -x $(BIAS_LIB_DIR)/libBias.a; ar -x $(BIAS_LIB_DIR)/liblr.a; ar -x $(BIAS_LIB_DIR)/libProfil.a; \
	rm Vector.o Matrix.o IntervalVector.o IntervalMatrix.o Utilities.o LSS.o; \
	ar -x $(SIMPLEX_LIB_DIR)/libsoplex.a; cd $(IBEX_DIR)

dyn_ibex_gaol: $(STATIC_TARGET)
	cd $(IBEX_LIB_DIR);  ar -x libibex.a; \
	ar -x $(GAOL_LIB_DIR)/libgaol.a; \
	ar -x $(SIMPLEX_LIB_DIR)/libsoplex.a; cd $(IBEX_DIR)

# For Linux
$(IBEX_LIB_DIR)/libibex.so: $(DYN_LIB_DEP)
	cd $(IBEX_LIB_DIR); g++ -shared *.o -o libibex.so; rm *.o; cd $(IBEX_DIR)

# For MacOS
$(IBEX_LIB_DIR)/libibex.dylib: $(DYN_LIB_DEP)
	cd $(IBEX_LIB_DIR); g++ -dynamiclib -o libibex.dylib *.o -lz; rm *.o; cd $(IBEX_DIR)

distrib:
	svn export https://ibex-lib.svn.sourceforge.net/svnroot/ibex-lib/branches/version2.0/distrib $(IBEX_DIR)/$(DISTRIB)
	install -m 640 $(IBEX_INC_DIR)/*.h* $(IBEX_DIR)/$(DISTRIB)/include ; \
	install -m 640 $(DYNAMIC_TARGET) $(IBEX_DIR)/$(DISTRIB)/lib ; \
	cd $(IBEX_DIR) ; \
	tar cvfz $(DISTRIB).tar.gz $(DISTRIB) ; \
	rm -Rf $(IBEX_DIR)/$(DISTRIB) ; \
	cd $(IBEX_SRC_DIR) 

.PHONY: distrib $(SUBDIRS) $(DYN_LIB_DEP)
	
	
clean: $(SUBDIRS)
	rm -f $(STATIC_TARGET) $(DYNAMIC_TARGET); \
	rm -f $(IBEX_INC_DIR)/*.h; \
	rm -f $(IBEX_INC_DIR)/ibex_$(SUBLIB)_Interval.h_; \
	rm -f $(DISTRIB).tar.gz
	
	