#include <exodus/program.h>
programinit()

#include <testqqqd2.h>
#include <initgeneral.h>

#include <win.h>
#include <gen.h>

function main() {
	printl("testqqqd says 'Hello World!'");
	//CREATE LABELLED COMMON
	mv.labelledcommon[1]=new win_common;

//	perform("initgeneral");
	call initgeneral();
 asm(" int $03");
	win.srcfile="";
	win.datafile="";
	win.orec="";
	win.wlocked="";
	win.reset="";
	win.valid="";

	call testqqqd2();
	return 0;
}

debugprogramexit()

