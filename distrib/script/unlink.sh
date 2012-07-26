#!/bin/sh

SOURCE_LINK=$1
TARGET_DIR=$2
TARGET_LINK=$3

# ============================================== 
if [ ! -d $TARGET_DIR ]; then
  echo Error: could not find $TARGET_DIR;
  exit 1
fi 

if [ ! -w $TARGET_DIR ]; then
  echo Error: you need super-user privilege \(\"sudo su make [un]install\"\);
  exit 1
fi 

if [ -L $TARGET_LINK ]; then
  echo -n \\t Unlinking $SOURCE_LINK ..........
  unlink $TARGET_LINK
  echo done.
else
  if [ -f $TARGET_LINK ]; then
    echo Error: corrupted installation \($TARGET_LINK should be a symbolic link\);
    exit 1
  fi
  if [ -d $TARGET_LINK ]; then
    echo Error: corrupted installation \($TARGET_LINK should be a symbolic link\);
    exit 1
  fi
fi

# ============================================== 


