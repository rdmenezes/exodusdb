//generated by exodus "compile ../med/plansubs9"
//#ifndef EXODUSDLFUNC_PLANSUBS9_H
#define EXODUSDLFUNC_PLANSUBS9_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_plansubs9;

//a member function with the right arguments, returning a var or void
var plansubs9(in msgno, out msg, in params="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_plansubs9.pmemberfunction_==NULL)
  efb_plansubs9.init("plansubs9","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,out,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_plansubs9.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_plansubs9.pmemberfunction_)))
  (msgno,msg,params);

}
//#endif
