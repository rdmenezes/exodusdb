#include <exodus/exodus.h>

function getparam(in result, in paramname, out paramvalue)
{
	var posn=index(result.ucase(),"\n"^paramname.ucase()^"=");
	if (not posn)
		return false;
	paramvalue=result.substr(posn+len(paramname)+2).field("\n",1);
	return true;
}

program() {
	
	var command=_COMMAND;

	//extract options
	var verbose=index(_OPTIONS,"V");
	var debugging=index(_OPTIONS,"B");
//	verbose=1;

	//extract filenames
	var filenames=field(command,FM,2,999999999);
	var nfiles=dcount(filenames,FM);
	if (not filenames)
		abort("Syntax is compile filename ... {options}");
	
	if (osgetenv("EXODUS_DEBUG"))
		debugging=true;

	//source extensions	
	var src_extensions="cpp cxx cc";
	var inc_extensions="h hpp hxx";
	var noncompilable_extensions=inc_extensions^ " out so o";
	var default_extension="cpp";

	var compiler;
	var basicoptions;
	var linkoptions;
	var binoptions;
	var liboptions;
	var bindir;
	var libdir;
	var installcmd;
	var outputoption="";
	var objfileextension;
	var binfileextension;
	var libfileextension;

	//hard coded compiler options at the moment
	//assume msvc (cl) on windows and g++ otherwise
	if (_SLASH=="/")
	{
		if (verbose)
			println("Posix environment detected. Assuming standard C++ compiler g++");

		compiler="g++";

		//basic compiler options
		basicoptions=" -Wall -ansi";
		//following will result in 2 byte wchar in linux
		//but all exodus libs also need to be in this format too
		//leave as 4 byte for now to avoid forcing additional compilation option on exodus users
		//who use g++ directly (or put runtime check that program+lib have the same size wchar
		//advising the right compilation option.
		//basicoptions^=" -fshort-wchar";

		//exodus library
		if (debugging)
			linkoptions=" -lexodus-gd";
		else
			linkoptions=" -lexodus";

		//enable function names in backtrace
		basicoptions^=" -rdynamic";

		outputoption=" -o ";
		//optimiser unfortunately prevents backtrace
		//basicoptions^="-O1";
		binoptions=" -g";
		//binoptions=" -fPIC";

		//make a shared library
		liboptions=" -shared";
		//soname?

		//target directories
		bindir="~/bin";
		libdir="~/lib";

		//target extensions
		objfileextension=".out";
		binfileextension="";
		libfileextension=".so";

		installcmd="mv";
	}
	else
	{
		if (verbose)
			println("Windows environment detected. Finding C++ compiler.");

		//get current environment
		var path=osgetenv("PATH");
		var include=osgetenv("INCLUDE");
		var lib=osgetenv("LIB");

		//locate MS Visual Studio by environment variable or current disk or C:
		var searchvars="CC VS90COMNTOOLS VS80COMNTOOLS VS70COMNTOOLS";
		if (verbose)
			searchvars.outputln("Searching Environment Variables : " ^ searchvars);

		var msvs;
		for (var ii=1;;++ii)
		{
			var envname=searchvars.field(" ",ii);
			if (not envname)
				break;
			msvs=osgetenv(envname);
			if (verbose)
				msvs.outputln(envname^" is ");
			if (msvs)
				break;
		}

		if (not msvs)
		{
			if (verbose)
				println("Searching standard directories");

			var searched=searchvars ^ " environment variables";
			var searchdirs="";

			//Visual Studio 2008 (Paid and Express)
			searchdirs^=FM ^ "\\Program Files\\Microsoft Visual Studio 9.0\\Common7\\Tools\\";

			//Visual Studio 2005
			searchdirs^=FM ^ "\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools\\";

			//http://syzygy.isl.uiuc.edu/szg/doc/VisualStudioBuildVars.html for VS6 and VS2003 folder info

			//Visual Studio .NET 2003
			searchdirs^= FM ^ "\\Program Files\\Microsoft Visual Studio .NET 2003\\Common7\\Tools\\";

			//Visual Studio 6.0
			searchdirs^= FM ^ "\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\";

			searchdirs.splicer(1,1,"");
			for (var ii=1;;++ii)
			{
				msvs=searchdirs.extract(ii);
				if (not msvs)
					break;
				if (osdir(msvs))
					break;
				searched^="\n" ^ msvs;
				msvs.splicer(1,0,"C:");
				if (osdir(msvs))
					break;
				searched^="\n" ^ msvs;
			}
			if (not msvs)
				abort("Searching for C++ Compiler:\n" ^ searched ^ "\nCannot find C++ compiler. Set environment variable CC to the MSVS COMMON TOOLS directory");
		}

		//call VSvars32 to get the path, include and lib
		if (msvs)
		{
			if (msvs.substr(-1) ne _SLASH)
				msvs^=_SLASH;
			//get lib/path/include from batch file
			var tempfilenamebase="comp$" ^rnd(99999999);
			var script="call " ^ (msvs ^ "vsvars32").quote();
			script^="\nset";
			if (verbose)
				println("Calling scriptscript");
			oswrite(script,tempfilenamebase^".cmd");
			osshell(tempfilenamebase ^ ".cmd > " ^ tempfilenamebase ^ ".$$$");
			var result;
			if (osread(result, tempfilenamebase^".$$$"))
			{
				//if (verbose)
				//	println(result);
				//println(result);
				result.converter("\r","\n");
				var value;
				if (getparam(result,"PATH",value))
					path=value;
				if (getparam(result,"INCLUDE",value))
					include=value;
				if (getparam(result,"LIB",value))
					lib=value;
				/*
				if (verbose)
				{
					println("\nPATH=",path);
					println("\nINCLUDE=",include);
					println("\nLIB=",lib);
				}
				*/

			}
			osdelete(tempfilenamebase^".cmd");
			osdelete(tempfilenamebase^".$$$");
		}

		if (verbose)
			println("Searching for Exodus for include and lib directories");

		//locate Exodus by executable path, environment variable or current disk or C:
		var ndeep=dcount(_EXECPATH,_SLASH);
		var exoduspath="";
		if (ndeep>2)
			exoduspath=_EXECPATH.field(_SLASH,1,ndeep-2);
		var searched="";
		if (not exoduspath or not (osfile(exoduspath^_SLASH^"bin\\exodus.dll") or osfile(exoduspath^_SLASH^"exodus.dll")))
		{
			if (exoduspath)
				searched^="\n"^exoduspath;
			exoduspath=osgetenv("EXODUS_PATH");
			if (not exoduspath)
			{
				searched^="\nEXODUS_PATH environment variable";
				exoduspath=L"\\Program Files\\exodus\\" EXODUS_RELEASE L"\\";
				if (not osdir(exoduspath))
				{
					searched^="\n" ^ exoduspath;
					exoduspath.splicer(1,0,"C:");
				}
			}
		}
		if (exoduspath.substr(-1,1) ne _SLASH)
			exoduspath^=_SLASH;
		var exodusbin=exoduspath;
		if (not osfile(exodusbin^"exodus.dll"))
		{
			searched^="\n" ^ exoduspath;
			exodusbin=exoduspath^"bin\\";
			if (not osfile(exodusbin^"exodus.dll"))
			{
				//exodusbin=_EXECPATH;
				searched^="\n" ^ exoduspath;
				println("Searching for Exodus",searched);
				abort("Cannot find Exodus. Set environment variable EXODUS_PATH to exodus directory and restart Exodus");
			}
		}

		/* now done by vcvarsall above
		var msvc=msvs^"..\\..\\VC\\";

		//add path to MSVC binaries
		path.splicer(0,0,";"^msvs^"..\\IDE;");
		path.splicer(0,0,";"^msvc^"bin;");

		//add msvc include
		include.splicer(0,0,";"^msvc^"include;");

		//add msvc lib
		lib.splicer(0,0,";"^msvc^"lib;");

		//express doesnt have win32
		if (not osfile(msvc^"lib\\kernel32.dll"))
		{
			//var libpath="\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Lib"
		}
		*/

		//add exodus path, include and lib
		path.splicer(0,0,exodusbin^";");
		include.splicer(0,0,exoduspath^"include;");
		include.splicer(0,0,exoduspath^"include\\exodus;");
		lib.splicer(0,0,exoduspath^"lib;");

		//set the new environment
		if (not ossetenv("PATH",path))
			println("Failed to set PATH environment variable");
		if (not ossetenv("INCLUDE",include))
			println("Failed to set INCLUDE  environment variable");
		if (not ossetenv("LIB",lib))
			println("Failed to set BIN environment variable");
		if (verbose)
		{
			println("\nPATH=",path);
			println("\nINCLUDE=",include);
			println("\nLIB=",lib);
		}
		compiler="cl";

		//basic compiler options
		basicoptions=" /EHsc /W3 /Zi /TP /D \"_CONSOLE\"";

		//exodus library
		if (debugging)
		{
			basicoptions^=" /O2 /GL /FD /MD /D \"NDEBUG\"";
		} else
		{
			basicoptions^=" /Od /GF /Gm /Gd /RTC1 /MDd /D \"DEBUG\"";
		}

		linkoptions=" /link exodus.lib";
		if (not verbose)
			linkoptions^=" /nologo";

		outputoption="";

		//enable function names in backtrace
		//basicoptions^=" -rdynamic";

		//make an exe
		binoptions="";

		//make a dll
		liboptions="";
		//soname?

		//target directories
		bindir="~/bin";
		libdir="~/lib";

		//target extensions
		objfileextension=".exe";
		binfileextension="exe";
		libfileextension=".dll";

		//installcmd="copy /y";
		installcmd="";
	}

	if (bindir.substr(-1) ne _SLASH)
		bindir ^= _SLASH;
	if (libdir.substr(-1)!=_SLASH)
		libdir ^= _SLASH;

	for (var fileno=1;fileno<=nfiles;++fileno) {
	
		var text="";
		var filebase;

		//get the next file name
		var srcfilename=filenames.extract(fileno).unquote();
		if (not srcfilename)
			continue;

		//println("--- ",srcfilename, " ---");
		
		//check/add the default file extension
		var fileext=srcfilename.field2(".",-1).lcase();
		if (src_extensions.locateusing(fileext," ")) {
			filebase=srcfilename.splice(-len(fileext)-1,999999,"");
		
		//skip non-compilable files
		} else if (noncompilable_extensions.locateusing(fileext," ")) {
			//errputln(srcfilename^" "^fileext^" is not compilable");
			continue;		

		//skip backup files
		} else if (fileext.substr(-1,1)=="~") {
			continue;

		//pickup default file if it exists - even if base file also exists
		} else if (osfile(srcfilename^"."^default_extension)) {
			filebase=srcfilename;
			srcfilename^="."^default_extension;
						
		//silently skip files that exist with the wrong header
		} else if (osfile(srcfilename) or osdir(srcfilename)) {
			continue;
			
		//add the default extension
		} else {
			filebase=srcfilename;
			srcfilename^="."^default_extension;
		
		}
		//get file text
		println(srcfilename);
		if (not text and not text.osread(srcfilename)) {
			errputln(srcfilename^" doesnt exist");
			continue;
		}

		//determine if program or subroutine/function
		var isprogram=index(text,"int main(") or index(text, "program()");
		
		var outputdir;
		var compileoptions;
		var binfilename=filebase;
		var objfilename=filebase;
		if (isprogram) {
			binfilename^=binfileextension;
			objfilename^=objfileextension;
			outputdir=bindir;
			compileoptions=binoptions;
		}else{
			binfilename^=binfileextension;
			objfilename^=libfileextension;
			outputdir=libdir;
			compileoptions=liboptions;
		}

		//work out libraries to include depending on included headers
		text.converter("\n\r",FM^FM);
		var line;
		var charn=1;
		var libnames="";
		var delimiter=0;
		do {
			line=text.remove(charn,delimiter).trim();
			var word1=line.field(" ",1);
			if (word1 eq "#include") {
				var word2=line.field(" ",2);
				if (word2.substr(1,1)==DQ) {
					word2=word2.substr(2,word2.len()-2);
					if (word2.substr(-2,2) eq ".h")
						word2.splicer(-2,2,"");
					libnames^=FM^word2;
				}
			}
		} while (delimiter);
		libnames.substrer(2).swapper(FM,", ");
		if (verbose)
			libnames.outputln("Libraries:");

		//record the current bin file update timestamp
		var oldobjfileinfo=osfile(objfilename);
		
		//build the compiler command
		var compilecmd=compiler ^ " " ^ srcfilename ^ basicoptions ^ compileoptions;
		if (outputoption)
			compilecmd^= outputoption ^ objfilename;
		compilecmd ^= linkoptions;
		//capture the output
		//compilecmd ^= " 2>&1 | tee " ^ srcfilename ^ ".err~";
		//no tee on windows so cannot monitor output at the moment until implement popen() call it osopen(cmd)?
		//similar tie syntax but different order
		var compileoutputfilename=srcfilename ^ ".~";
		if (_SLASH eq "/")
			compilecmd ^= " 2>&1 | tee " ^ compileoutputfilename.quote();
		else
			compilecmd ^= " > " ^ compileoutputfilename.quote() ^ " 2>&1";

		//call the compiler
		///////////////////
		if (verbose or debugging)
			println(compilecmd);
		osshell(compilecmd);
		
		//handle compiler output
		var compileroutput;
		var startatlineno;
		if (osread(compileroutput,compileoutputfilename)){
			outputln(compileroutput);
			osdelete(compileoutputfilename);
			var charn=index(compileroutput, ": error:");
			if (charn) {
				startatlineno=compileroutput.substr(charn-9,9);
				startatlineno=startatlineno.field2(":",-1);
//				print("ERROR(S) FOUND IN " ^ srcfilename ^ " STARTING IN LINE "^startatlineno^" ... ");
//				var().input(1);
				continue;
			}
		}
		   
		//get new objfile info or continue
		var newobjfileinfo=osfile(objfilename);
		if (not objfilename) {
				print("CANNOT FIND OUTPUT FILE");
				var().input(1);
				continue;
		}
		
		//if new objfile			
		if (newobjfileinfo not_eq oldobjfileinfo) {
			if (newobjfileinfo) {

				//create the bin file directory if missing
				//osdir isnt working as expected
				//                              if (!outputdir.oslistf()) {
				//                                      osshell("mkdir " ^ outputdir);
				//                                      println("Created " ^ outputdir);
				//                              }
				
				//var outputfilename=filename;
				//if (isprogram) {
        			//	//remove the .out file type ending
        			//	outputfilename=outputfilename.field(".",1,outputfilename.count("."));
				//}
				
				//copy the obj file to the output directory
				if (installcmd) {
					var cmd=installcmd^" " ^ objfilename ^ " " ^ outputdir ^ binfilename;
					if (verbose)
						println(cmd);
					osshell(cmd);
				}
			}
		}//compilation
	}//fileno
}