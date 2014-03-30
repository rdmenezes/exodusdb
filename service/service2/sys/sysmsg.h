#ifndef EXODUSDLFUNC_SYSMSG_H
#define EXODUSDLFUNC_SYSMSG_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_sysmsg;

//a member function with the right arguments, returning a var or void
var sysmsg(in msg0, in subject0="", in username0="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_sysmsg.pmemberfunction_==NULL)
  efb_sysmsg.init("sysmsg","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_sysmsg.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_sysmsg.pmemberfunction_)))
  (msg0,subject0,username0);

}
#endif
