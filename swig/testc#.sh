#!/bin/bash
set -e

#we installed jexodus.jar and jexodus.so in the general library
#because java has no standard place for modules and libraries
export LOCAL_LIBDIR=/usr/local/lib
test -d ${LOCAL_LIBDIR}64 && export LOCAL_LIBDIR=${LOCAL_LIBDIR}64
export EXO_MOD_PATH=$LOCAL_LIBDIR
export EXO_LIB_PATH=$LOCAL_LIBDIR


#delete any existing test.exe
test -f test.exe && rm -f test.exe && echo rm -f test.exe

#--------
# Compile
#--------
echo \
gmcs test.cs -r:$EXO_MOD_PATH/exodus_library
gmcs test.cs -r:$EXO_MOD_PATH/exodus_library

#----
# Run
#----
#mono xxx command not necessary in later versions can execute .exe from command line
echo \
env MONO_PATH=$EXO_MOD_PATH env LD_LIBRARY_PATH=$EXO_LIB_PATH mono ./test.exe
env MONO_PATH=$EXO_MOD_PATH env LD_LIBRARY_PATH=$EXO_LIB_PATH mono ./test.exe
