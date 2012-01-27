include common.mak

TARGET  :=$(IBEX_LIB_DIR)/libibex.a
TOOLS   :=tools
LEVEL0  :=level0
LEVEL1  :=level1
SUBDIRS :=$(TOOLS) $(LEVEL0) $(LEVEL1) 

OBJT     =$(wildcard $(IBEX_DIR)/src/$(TOOLS)/*.o) 
OBJ0     =$(wildcard $(IBEX_DIR)/src/$(LEVEL0)/*.o) 
OBJ1     =$(wildcard $(IBEX_DIR)/src/$(LEVEL1)/*.o)
OBJS     =$(OBJT) $(OBJ0) $(OBJ1)

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