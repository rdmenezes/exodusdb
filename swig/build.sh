#!/bin/bash
set -e

export SWIG_TARGET=$1

export EXO_EXODUS_INCLUDE_FLAGS="-I../../exodus/exodus"
export EXO_EXODUS_LDFLAGS="-lexodus"
#defaults
export SWIG_WRAPPER_EXT=cpp
export SWIG_MODULE_FILENAME="exodus.so"

#----------------
#--- "Target" ---
#----------------
case $SWIG_TARGET in
php )
        export SWIG_TARGET_INCLUDE_FLAGS="`php-config --includes`"
        export SWIG_TARGET_LIBDIR="`php-config --extension-dir`"
;;
python )
        export SWIG_TARGET_INCLUDE_FLAGS="`python-config --includes`"
        export SWIG_WRAPPER_EXT=cxx
	export SWIG_TARGET_LDFLAGS="-lpython2.6"
	export SWIG_MODULE_FILENAME="_exodus.so"
;;
java )
        export SWIG_TARGET_INCLUDE_FLAGS="-I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux"
        export SWIG_MODULE_FILENAME="libexodus.so"
;;
*)
        echo "Invalid or Missing SWIG target, $SWIG_TARGET"
        exit 1
;;
esac

#------------
#--- "cd" ---
#------------

test -d $SWIG_TARGET || mkdir $SWIG_TARGET
cd $SWIG_TARGET

#--------------
#--- "Swig" ---
#--------------
echo swig -c++ -$SWIG_TARGET $EXO_EXODUS_INCLUDE_FLAGS -outcurrentdir ../exodus.i
     swig -c++ -$SWIG_TARGET $EXO_EXODUS_INCLUDE_FLAGS -outcurrentdir ../exodus.i

#-----------------
#--- "Compile" ---
#-----------------
echo Compiling
#g++ -I/usr/include/python2.6 -fPIC -c exodus_wrap.cxx
#g++ -shared exodus_wrap.o -o exodus.so -L.libs -lexodus -lpython2.6 -Wl,-no-undefined


gcc -c exodus_wrap.$SWIG_WRAPPER_EXT -fPIC $SWIG_TARGET_INCLUDE_FLAGS $EXO_EXODUS_INCLUDE_FLAGS

g++ -shared exodus_wrap.o -o $SWIG_MODULE_FILENAME $EXO_EXODUS_LDFLAGS $SWIG_TARGET_LDFLAGS


#-----------------
#--- "Install" ---
#-----------------
test "$SWIG_TARGET_LIBDIR" == "" && exit
sudo cp -f $SWIG_MODULE_FILENAME $SWIG_TARGET_LIBDIR/
