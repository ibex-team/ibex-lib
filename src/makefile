include common.mak

TARGET:=$(IBEX_LIB_DIR)/libibex.a
ARITH:=arithmetic
SUBDIRS:=tools $(ARITH) numeric symbolic combinatorial contractor bisector strategy
SUBLIB_DEF:=_IBEX_WITH_`echo $(SUBLIB) | tr '[:lower:]' '[:upper:]'`_

# warning: don't define OBJS with ":=" because the
# object files may not exist yet
OBJS  =$(wildcard $(IBEX_SRC_DIR)/tools/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/arithmetic/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/numeric/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/symbolic/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/combinatorial/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/contractor/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/bisector/*.o) \
       $(wildcard $(IBEX_SRC_DIR)/strategy/*.o)       

all : $(TARGET)

$(TARGET) : headers $(SUBDIRS)
	ar rcs $@ $(OBJS);
     
headers :
	echo \/\* this file is generated \*\/ > $(ARITH)/ibex_bias_or_gaol.h ; \
	echo \#define $(SUBLIB_DEF) >> $(ARITH)/ibex_bias_or_gaol.h; \
	find $(IBEX_SRC_DIR) -name "*.h" | while read LINE; do install -m 640 $${LINE} $(IBEX_INC_DIR)/`basename $${LINE}`; done

clean: $(SUBDIRS)
	rm -f $(TARGET); \
	find $(IBEX_INC_DIR)/*.h -exec rm -f {} \; ; \
	
.PHONY: $(SUBDIRS)
 
$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@ 

