//#ifndef EXODUSDLFUNC_AUTHORISED_H
#define EXODUSDLFUNC_AUTHORISED_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_authorised;

//a member function with the right arguments, returning a var or void
var authorised(in task0, out msg, in defaultlock="", in username0="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_authorised.pmemberfunction_==NULL)
  efb_authorised.init("authorised","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,out,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_authorised.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_authorised.pmemberfunction_)))
  (task0,msg,defaultlock,username0);

}
//#endif
