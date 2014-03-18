#ifndef EXODUSDLFUNC_READACCPARAMS_H
#define EXODUSDLFUNC_READACCPARAMS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_readaccparams;

//a member function with the right arguments, returning a var
var readaccparams()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_readaccparams.pmemberfunction_==NULL)
  efb_readaccparams.init("readaccparams","exodusprogrambasecreatedelete",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args, returning a var
 return CALLMEMBERFUNCTION(*(efb_readaccparams.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_readaccparams.pmemberfunction_)))
  ();

}
#endif
