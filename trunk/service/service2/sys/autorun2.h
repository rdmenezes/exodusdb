//#ifndef EXODUSDLFUNC_AUTORUN2_H
#define EXODUSDLFUNC_AUTORUN2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_autorun2;

//a member function with the right arguments, returning a var or void
var autorun2(in mode, in title0, in module, in request, in data, in runasusercode0, in targetusercodes0, io document0, io docid, out msg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_autorun2.pmemberfunction_==NULL)
  efb_autorun2.init("autorun2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in,in,in,in,io,io,out);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_autorun2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_autorun2.pmemberfunction_)))
  (mode,title0,module,request,data,runasusercode0,targetusercodes0,document0,docid,msg);

}
//#endif
