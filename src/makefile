include common.mak

STATIC_TARGET:=$(IBEX_LIB_DIR)/libibex.a
DYNAMIC_TARGET:=$(IBEX_LIB_DIR)/libibex.so
ARITH:=arithmetic
SUBDIRS:=tools $(ARITH) numeric symbolic function combinatorial contractor bisector strategy parser java
SUBLIB_DEF:=_IBEX_WITH_`echo $(SUBLIB) | tr '[:lower:]' '[:upper:]'`_

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

ifeq ($(SUBLIB), gaol)
DYNLIBS=
else
DYNLIBS=dyn_ibex_bias  
endif

all : static

static : $(STATIC_TARGET)

dynamic : $(DYNLIBS)

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

#$(MAKS) $(MAKECMDGOALS) ?

$(SUBDIRS):
	$(MAKE) all -C $@ 

dyn_ibex_bias : $(STATIC_TARGET)
	cd $(IBEX_LIB_DIR);  ar -x libibex.a; \
	ar -x $(BIAS_LIB_DIR)/libBias.a; ar -x $(BIAS_LIB_DIR)/liblr.a; ar -x $(BIAS_LIB_DIR)/libProfil.a; \
	ar -x $(SIMPLEX_LIB_DIR)/libsoplex.a; \
	g++ -shared *.o -o libibex.so; rm *.o; cd $(IBEX_DIR)/$(CHOCO)

.PHONY: $(SUBDIRS) dyn_ibex_bias

clean: $(SUBDIRS)
	rm -f $(STATIC_TARGET) $(DYNAMIC_TARGET); \
	find $(IBEX_INC_DIR)/*.h -exec rm -f {} \; ; \
	rm -f $(IBEX_INC_DIR)/ibex.h
	