//generated by exodus "compile singular"
//#ifndef EXODUSDLFUNC_SINGULAR_H
#define EXODUSDLFUNC_SINGULAR_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_singular;

//a member function with the right arguments, returning a var or void
var singular(in pluralnoun)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_singular.pmemberfunction_==NULL)
  efb_singular.init("singular","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_singular.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_singular.pmemberfunction_)))
  (pluralnoun);

}
//#endif
