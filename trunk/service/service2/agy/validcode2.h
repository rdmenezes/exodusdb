//generated by exodus "compile ../agy/validcode2.cpp"
//#ifndef EXODUSDLFUNC_VALIDCODE2_H
#define EXODUSDLFUNC_VALIDCODE2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_validcode2;

//a member function with the right arguments, returning a var or void
var validcode2(in companycodex, in clientcodex, in brandcodex, io brands, out msg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_validcode2.pmemberfunction_==NULL)
  efb_validcode2.init("validcode2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,io,out);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_validcode2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_validcode2.pmemberfunction_)))
  (companycodex,clientcodex,brandcodex,brands,msg);

}
//#endif
