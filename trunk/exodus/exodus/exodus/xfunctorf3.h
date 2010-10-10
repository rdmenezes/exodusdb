#ifndef ExodusFunctorF3_H
#define ExodusFunctorF3_H

//generated by genfunctors.cpp

#include <exodus/mvfunctor.h>

template<class T1, class T2, class T3>
class ExodusFunctorF3 : private ExodusFunctorBase
{
public:
 ExodusFunctorF3(const std::string libname,const std::string funcname)
	: ExodusFunctorBase(libname,funcname){}

 var operator() ()
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3);
	return  ((ExodusDynamic) _pfunction)(T1(), T2(), T3());
 }

 var operator() (T1 arg1)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3);
	return  ((ExodusDynamic) _pfunction)(arg1, T2(), T3());
 }

 var operator() (T1 arg1, T2 arg2)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3);
	return  ((ExodusDynamic) _pfunction)(arg1, arg2, T3());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3);
	return  ((ExodusDynamic) _pfunction)(arg1, arg2, arg3);
 }

};
#endif