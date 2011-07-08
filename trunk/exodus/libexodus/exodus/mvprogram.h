#ifndef MVPROGRAM_H
#define MVPROGRAM_H

#include <exodus/mvenvironment.h>
//#include <exodus/mvfunctor.h>

namespace exodus {

class ExodusProgramBase
{
public:

DLL_PUBLIC
	ExodusProgramBase(MvEnvironment& inmv);

//doing virtual isnt much use because external functions (which are based on ExodusProgramBase)
//need to have complete freedom of arguments to main(...)
//virtual var main();

DLL_PUBLIC
	virtual ~ExodusProgramBase();

	//mv.xyz is going to be used a lot by exodus programmers for exodus "global variables"
	//eg mv.RECORD mv.DICT
	//
	//threadsafe! it is member data so it is global to the class/object and not global to the program
	//
	//it is a reference/pointer so that an external "subroutine" can be created which has identical
	//exodus global variables to the "main program" as far as the exodus application programmer thinks
	//
	//being a reference ensures that exodus programs cannot exist without an mv
	//however this restriction might be relaxed
	//
	//mv was initially a reference so that exodus application programmers could writew
	//things like mv.ID (instead of the harder to understand, for an application programmer, mv->ID style)
	//however now that a macro is used to generate mv.ID from just ID we could make
	//mv to be a pointer and assign it as and when desired
	//we would just need to change
	//#define ID mv.ID
	//to
	//#define ID mv->ID
	//so that ID RECORD etc. continue to appear to the application programmer to be "threadsafe global" variables
	MvEnvironment& mv;

};

}
#endif //MVPROGRAM_H