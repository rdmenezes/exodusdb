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

	void note(const var& msg, const var& options="") const;
	void note(const var& msg, const var& options, var& buffer, const var& params="") const;
	void mssg(const var& msg, const var& options="") const;
	void mssg(const var& msg, const var& options, var& buffer, const var& params="") const;

	var authorised(const var& task0, var& msg, const var& defaultlock="", const var& username0="");
	var authorised(const var& task0);
	void readuserprivs() const;
	void writeuserprivs() const;

	var capitalise(const var& str0, const var& mode=var(), const var& wordseps=var()) const;

	var perform(const var& sentence);
	var execute(const var& sentence);

	//given dictid reads dictrec from DICT file and extracts from RECORD/ID or calls library called dict+DICT function dictid
	//not const so we can mess with the library?
	var calculate(const var& dictid);
	var calculate(const var& dictid, const var& dictfile, const var& id, const var& record, const var& mv=0);

	//moved to exodusprogrambase
	//var perform(const var& sentence);

	//ditto
	//given dictid reads dictrec from DICT file and extracts from RECORD/ID or calls library called dict+DICT function dictid
	//not const so we can mess with the library?
	//var calculate(const var& dictid);
	//var calculate(const var& dictid, const var& dictfile, const var& id, const var& record, const var& mv=0);

	var otherusers(const var& param);
	var otherdatasetusers(const var& param);

	//moved to mvprogram
	//var capitalise(const var& str0, const var& mode=var(), const var& wordseps=var()) const;
	////var capitalise(const var& str0, const var& mode=L"", const var& wordseps=L"") const;

	void debug() const;
	void fsmsg() const;
	var sysvar(const var& var1,const var& var2,const var& mv3,const var& mv4);
	void setprivilege(const var& var1);
	bool openfile(const var& filename, var& file) const;
	bool openfile2(const var& filename, var& file, const var& similarfilename, const var& autocreate=L"") const;

	//NB does not return record yet
	bool lockrecord(const var& filename, const var& file, const var& keyx, const var& recordx, const int waitsecs=0, const bool allowduplicate=false) const;
	//bool lockrecord(const var& xfilename, const var& xfile, const var& keyx, const var& recordx, const var& waitsecs, const bool allowduplicate=false) const;
	bool lockrecord(const var& filename, const var& file, const var& keyx) const;
	bool unlockrecord(const var& filename, const var& file, const var& key) const;

	var decide(const var& question, const var& options) const;
	var decide(const var& question, const var& options, var& reply, const int defaultreply=0) const;

	void savescreen(var& origscrn, var& origattr) const;
	//void ostime(var& ostimenow) const;
	int keypressed(int delayusecs=0) const;
	bool esctoexit() const;

	bool oswritex(const var& str, const var& filename) const;
	bool osbwritex(const var& str1, const var& filehandle, const var& filename, var& offset) const;
//	bool osbreadx(var& str1, const var& filehandle, const var& filename, const int startoffset, const int length);
	bool osbreadx(var& str1, const var& filehandle, const var& filename, var& startoffset, const int length);

	//moved to external function
	//bool authorised(const var& task, var& msg, const var& defaultlock=L"");
	//bool authorised(const var& task);
	//void readuserprivs();
	//void writeuserprivs();
	//void sysmsg(const var& msg);
	//var sendmail(const var& toaddress, const var& subject, const var& body0, const var& attachfilename, const var& deletex, var& errormsg);

	void logger(const var& programname, const var& logtext);
	var singular(const var& pluralnoun);
	void flushindex(const var& filename);
	var encrypt2(const var& encrypt0) const;
	var xmlquote(const var& str) const;
	var loginnet(const var& dataset, const var& username, var& cookie, var& msg);
	var AT(const int code) const;
	var AT(const int x, const int y) const;

    //was MVDB
	var getuserdept(const var& usercode);

private:

	//used by calculate to call dict libraries
	mutable ExodusFunctorBase dict_exodusfunctorbase_;
	//TODO cache many not just one
	mutable var cache_dictid_;
	mutable var cache_dictrec_;

	//used by perform to call libraries
	mutable ExodusFunctorBase perform_exodusfunctorbase_;
	//TODO cache many not just one
	mutable var cache_perform_libid_;

};

}
#endif //MVPROGRAM_H
