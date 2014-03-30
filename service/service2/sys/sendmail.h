#ifndef EXODUSDLFUNC_SENDMAIL_H
#define EXODUSDLFUNC_SENDMAIL_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_sendmail;

//a member function with the right arguments, returning a var or void
var sendmail(in toaddress0, in ccaddress0, in subject0, in body0, in attachfilename0, in delete0, out errormsg, in replyto0=var(), in params0=var())
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_sendmail.pmemberfunction_==NULL)
  efb_sendmail.init("sendmail","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in,in,in,out,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_sendmail.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_sendmail.pmemberfunction_)))
  (toaddress0,ccaddress0,subject0,body0,attachfilename0,delete0,errormsg,replyto0,params0);

}
#endif