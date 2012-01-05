include common.mak

TARGET  :=$(IBEX_LIB_DIR)/libibex.a
TOOLS   :=tools
LEVEL0  :=level0
SUBDIRS :=$(TOOLS) $(LEVEL0) 

OBJT     =$(wildcard $(IBEX_DIR)/src/$(TOOLS)/*.o) 
OBJ0     =$(wildcard $(IBEX_DIR)/src/$(LEVEL0)/*.o) 
OBJS     =$(OBJT) $(OBJ0) 

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