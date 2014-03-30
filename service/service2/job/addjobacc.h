#ifndef EXODUSDLFUNC_ADDJOBACC_H
#define EXODUSDLFUNC_ADDJOBACC_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_addjobacc;

//a member function with the right arguments, returning a var or void
var addjobacc(in addaccmode, in jobno, io msg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_addjobacc.pmemberfunction_==NULL)
  efb_addjobacc.init("addjobacc","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_addjobacc.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_addjobacc.pmemberfunction_)))
  (addaccmode,jobno,msg);

}
#endif