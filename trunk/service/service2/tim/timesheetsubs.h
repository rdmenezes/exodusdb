//#ifndef EXODUSDLFUNC_TIMESHEETSUBS_H
#define EXODUSDLFUNC_TIMESHEETSUBS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_timesheetsubs;

//a member function with the right arguments, returning a var or void
var timesheetsubs(in mode0)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_timesheetsubs.pmemberfunction_==NULL)
  efb_timesheetsubs.init("timesheetsubs","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_timesheetsubs.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_timesheetsubs.pmemberfunction_)))
  (mode0);

}
//#endif