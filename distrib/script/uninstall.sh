#!/bin/sh

echo uninstalling ibex...

IBEXPATH=`pwd`

TARGET_DIR=/usr/local/include
TARGET_LINK=$TARGET_DIR/ibex
SOURCE_LINK=$IBEXPATH/include

if [ "$KERNEL" == "Linux" ]; then
  echo OS identified as Linux;
  DYLIB=libibex.so
elif [ "$KERNEL" == "Darwin" ]; then
  echo OS identified as Darwin;
  DYLIB=libibex.dylib
else 
  echo Unknown architecture: $KERNEL
  exit 1
fi

# ============================================== 
./script/unlink.sh $SOURCE_LINK $TARGET_DIR $TARGET_LINK 

if [ $? != 0 ]; then
    exit
fi
# ============================================== 

TARGET_DIR=/usr/local/lib
TARGET_LINK=$TARGET_DIR/$DYLIB
SOURCE_LINK=$IBEXPATH/lib/$DYLIB

# ============================================== 
./script/unlink.sh $SOURCE_LINK $TARGET_DIR $TARGET_LINK 

if [ $? != 0 ]; then
    exit
fi
# ============================================== 

if [ "$KERNEL" == "Linux" ]; then
  echo -n \\t Running ldconfig ..........
  ldconfig
  echo done.
endif

echo done.
