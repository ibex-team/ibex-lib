include common.mak

TARGET  :=$(IBEX_LIB_DIR)/libibex.a
SUBDIRS :=tools arithmetic numeric symbolic combinatorial contractor

OBJS :=$(wildcard $(IBEX_DIR)/src/tools/*.o) \
       $(wildcard $(IBEX_DIR)/src/arithmetic/*.o) \
       $(wildcard $(IBEX_DIR)/src/numeric/*.o) \
       $(wildcard $(IBEX_DIR)/src/symbolic/*.o) \
       $(wildcard $(IBEX_DIR)/src/combinatorial/*.o) \
       $(wildcard $(IBEX_DIR)/src/contractor/*.o)

all : $(TARGET)

clean : subdirs

$(TARGET) : subdirs
	ar rcs $@ $(OBJS);
     
.PHONY: subdirs $(SUBDIRS)
 
subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@ 

clean:
	rm -f $(TARGET) 