//#ifndef EXODUSDLFUNC_DICT_MARKETS_H
#define EXODUSDLFUNC_DICT_MARKETS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_markets;

//a member function with the right arguments, returning a var or void
var dict_markets()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_markets.pmemberfunction_==NULL)
  efb_dict_markets.init("dict_markets","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_markets.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_markets.pmemberfunction_)))
  ();

}

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_markets;

//a member function with the right arguments, returning a var or void
var dict_markets()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_markets.pmemberfunction_==NULL)
  efb_dict_markets.init("dict_markets","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_markets.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_markets.pmemberfunction_)))
  ();

}

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_markets;

//a member function with the right arguments, returning a var or void
var dict_markets()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_markets.pmemberfunction_==NULL)
  efb_dict_markets.init("dict_markets","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_markets.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_markets.pmemberfunction_)))
  ();

}

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_markets;

//a member function with the right arguments, returning a var or void
var dict_markets()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_markets.pmemberfunction_==NULL)
  efb_dict_markets.init("dict_markets","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_markets.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_markets.pmemberfunction_)))
  ();

}

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_dict_markets;

//a member function with the right arguments, returning a var or void
var dict_markets()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_dict_markets.pmemberfunction_==NULL)
  efb_dict_markets.init("dict_markets","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_dict_markets.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_dict_markets.pmemberfunction_)))
  ();

}
//#endif
