#ifndef EXODUSDLFUNC_GETACC_H
#define EXODUSDLFUNC_GETACC_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_getacc;

//a member function with the right arguments, returning a var or void
var getacc(io mode, io validaccs, io reqcompanies2, io acctype, io reqledgertype, io getacc2, io inpaccnos)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_getacc.pmemberfunction_==NULL)
  efb_getacc.init("getacc","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,io,io,io,io,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_getacc.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_getacc.pmemberfunction_)))
  (mode,validaccs,reqcompanies2,acctype,reqledgertype,getacc2,inpaccnos);

}
#endif