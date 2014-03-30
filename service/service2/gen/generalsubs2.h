#ifndef EXODUSDLFUNC_GENERALSUBS2_H
#define EXODUSDLFUNC_GENERALSUBS2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_generalsubs2;

//a member function with the right arguments, returning a var or void
var generalsubs2(in mode)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_generalsubs2.pmemberfunction_==NULL)
  efb_generalsubs2.init("generalsubs2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_generalsubs2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_generalsubs2.pmemberfunction_)))
  (mode);

}
#endif
