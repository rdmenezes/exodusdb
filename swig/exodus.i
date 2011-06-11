%module(docstring="An interface to Exodus.") exodus

#ifdef SWIGPYTHON

%include "python/pywstrings.swg"
%include "python/pystrings.swg"
%include "python/std_string.i"
%include "python/typemaps.i"  

%fragment("SWIG_AsVal_wchar_t", "header", fragment="<wchar.h>") {
    SWIGINTERN int SWIG_AsVal_wchar_t(PyObject* p, wchar_t* c) {
        return SWIG_OK;
    }
}
%fragment("SWIG_From_wchar_t", "header", fragment="<wchar.h>") {
    SWIGINTERNINLINE PyObject* SWIG_From_wchar_t(wchar_t c) {
        return SWIG_Py_Void();
    }
} 

/*
// Python -> C
%typemap(in) wchar_t const * {
  $1 = PyString_to_wchar_t($input);
}

// C -> Python
%typemap(out) wchar_t * {
  $result = wchar_t_to_PyObject($1);
}
*/

#endif

#ifdef SWIGPHP
%include "php/php.swg"
//%include "php/phpinit.swg"
////%include "php/phprun.swg"
//%include "php/phpkw.swg"
%include "php/typemaps.i"
%include "php/std_string.i"
//%include "exception.i"
//%include "php/utils.i"

%rename(__toString) tostring;

#endif

%{
#include "exodus/mv.h"
%}

%include "exodus/mv.h"