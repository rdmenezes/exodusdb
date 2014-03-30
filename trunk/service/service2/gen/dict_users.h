//#ifndef EXODUSDLFUNC_DICT_USERS_H
#define EXODUSDLFUNC_DICT_USERS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_users;

//a member function with the right arguments, returning a var or void
var dict_users()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_users.pmemberfunction_==NULL)
  efb_dict_users.init("dict_users","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_users.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_users.pmemberfunction_)))
  ();

}

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_users;

//a member function with the right arguments, returning a var or void
var dict_users()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_users.pmemberfunction_==NULL)
  efb_dict_users.init("dict_users","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_users.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_users.pmemberfunction_)))
  ();

}
//#endif
