#ifndef EXODUSDLFUNC_AGENCYSUBS_H
#define EXODUSDLFUNC_AGENCYSUBS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_agencysubs;

//a member function with the right arguments, returning a var or void
var agencysubs(in mode, io msg, in param3x="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_agencysubs.pmemberfunction_==NULL)
  efb_agencysubs.init("agencysubs","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_agencysubs.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_agencysubs.pmemberfunction_)))
  (mode,msg,param3x);

}
#endif
