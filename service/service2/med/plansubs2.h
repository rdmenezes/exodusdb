#ifndef EXODUSDLFUNC_PLANSUBS2_H
#define EXODUSDLFUNC_PLANSUBS2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_plansubs2;

//a member function with the right arguments, returning a var or void
var plansubs2(in modex)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_plansubs2.pmemberfunction_==NULL)
  efb_plansubs2.init("plansubs2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_plansubs2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_plansubs2.pmemberfunction_)))
  (modex);

}
#endif
