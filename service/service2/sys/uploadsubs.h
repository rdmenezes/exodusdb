//generated by exodus "compile uploadsubs"
//#ifndef EXODUSDLFUNC_UPLOADSUBS_H
#define EXODUSDLFUNC_UPLOADSUBS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_uploadsubs;

//a member function with the right arguments, returning a var or void
var uploadsubs(in mode)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_uploadsubs.pmemberfunction_==NULL)
  efb_uploadsubs.init("uploadsubs","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_uploadsubs.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_uploadsubs.pmemberfunction_)))
  (mode);

}
//#endif
