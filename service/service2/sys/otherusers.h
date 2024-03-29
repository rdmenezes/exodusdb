//generated by exodus "compile otherusers"
//#ifndef EXODUSDLFUNC_OTHERUSERS_H
#define EXODUSDLFUNC_OTHERUSERS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_otherusers;

//a member function with the right arguments, returning a var or void
var otherusers(in databasecode="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_otherusers.pmemberfunction_==NULL)
  efb_otherusers.init("otherusers","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_otherusers.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_otherusers.pmemberfunction_)))
  (databasecode);

}
//#endif
