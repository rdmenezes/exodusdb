//generated by exodus "compile select2"
//#ifndef EXODUSDLFUNC_SELECT2_H
#define EXODUSDLFUNC_SELECT2_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_select2;

//a member function with the right arguments, returning a var or void
var select2(in filenamex, in linkfilename2, in sortselect0, in dictids0, in options, io datax, io response, in limitfields0="", in limitchecks="", in limitvalues="", int maxnrecs=0)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_select2.pmemberfunction_==NULL)
  efb_select2.init("select2","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in,in,io,io,in,in,in,int);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_select2.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_select2.pmemberfunction_)))
  (filenamex,linkfilename2,sortselect0,dictids0,options,datax,response,limitfields0,limitchecks,limitvalues,maxnrecs);

}
//#endif
