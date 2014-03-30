#ifndef EXODUSDLFUNC_PLANSUBS3B_H
#define EXODUSDLFUNC_PLANSUBS3B_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_plansubs3b;

//a member function with the right arguments, returning a var or void
var plansubs3b(in modex)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_plansubs3b.pmemberfunction_==NULL)
  efb_plansubs3b.init("plansubs3b","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_plansubs3b.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_plansubs3b.pmemberfunction_)))
  (modex);

}
#endif
