#ifndef ExodusFunctorF9_H
#define ExodusFunctorF9_H

//generated by genfunctors.cpp

#include <exodus/mvfunctor.h>

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class ExodusFunctorF9 : private ExodusFunctorBase
{
public:
 ExodusFunctorF9(const std::string libname,const std::string funcname)
	: ExodusFunctorBase(libname,funcname){}

 var operator() ()
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(T1(), T2(), T3(), T4(), T5(), T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, T2(), T3(), T4(), T5(), T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, T3(), T4(), T5(), T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, T4(), T5(), T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, T5(), T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5, T6(), T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5, arg6, T7(), T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, T8(), T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, T9());
 }

 var operator() (T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
 {
	checkload();
	typedef var (*ExodusDynamic)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	return  ((ExodusDynamic) pfunction_)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
 }

};
#endif
