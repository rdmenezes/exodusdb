//generated by exodus "compile loginnet"
//#ifndef EXODUSDLFUNC_LOGINNET_H
#define EXODUSDLFUNC_LOGINNET_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_loginnet;

//a member function with the right arguments, returning a var or void
var loginnet(in dataset, in username, io cookie, io msg, io authcompcodes)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_loginnet.pmemberfunction_==NULL)
  efb_loginnet.init("loginnet","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_loginnet.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_loginnet.pmemberfunction_)))
  (dataset,username,cookie,msg,authcompcodes);

}
//#endif
