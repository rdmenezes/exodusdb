//#ifndef EXODUSDLFUNC_LOG2_H
#define EXODUSDLFUNC_LOG2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_log2;

//a member function with the right arguments, returning a var or void
var log2(in msg0, io time0)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_log2.pmemberfunction_==NULL)
  efb_log2.init("log2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_log2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_log2.pmemberfunction_)))
  (msg0,time0);

}
//#endif
