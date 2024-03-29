#ifndef ExodusFunctorF5_H
#define ExodusFunctorF5_H

//generated by genfunctors.cpp

#include <exodus/mvfunctor.h>

template<class T1, class T2, class T3, class T4, class T5>
class ExodusFunctorF5 : private ExodusFunctorBase
{
public:
 ExodusFunctorF5(const std::string libname,const std::string funcname)
	: ExodusFunctorBase(libname,funcname){}

 var operator() ()
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(T1(), T2(), T3(), T4(), T5());
 }

 var operator() (T1 arg1)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(arg1, T2(), T3(), T4(), T5());
 }

 var operator() (T1 arg1, T2 arg2)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, T3(), T4(), T5());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, T4(), T5());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, T5());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5);
 }

};
#endif
