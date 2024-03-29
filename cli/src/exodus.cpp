/*
Copyright (c) 2009 steve.bush@neosys.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <exodus/exodus.h>

program()
{

        printl("Exodus Copyright (c) 2009 steve.bush@neosys.com");
        printl("http://www.opensource.org/licenses/mit-license.php");
		printl("Using Library Version: ", var().version(), " ", PLATFORM_);

        var verbose=OPTIONS.index("V");

	if (verbose)
		EXECPATH.outputl("Executable:");

        var exodusbinpath=field(EXECPATH,SLASH,1,dcount(EXECPATH,SLASH)-1);

	if (verbose)
		exodusbinpath.outputl("Path:");

        //if (not var().load("libpq.dll"))
        //	printl("Warning: Cannot find libpq.dll to connect to postgres");

        var command=SENTENCE.field(" ",2,999999);
        var shell;

	//non-windows
        if (SLASH eq "/" and shell.osgetenv("SHELL")) {

                var home=osgetenv("HOME");
                var path=osgetenv("PATH");
                var libp=osgetenv("LD_LIBRARY_PATH");

                //prefer user binaries then exodus binaries AFTER existing path
				ossetenv("PATH",path^":"^home^"/bin:/var/lib/exodus/bin");

//print("LD_LIBRARY_PATH","~/lib:"^osgetenv("LD_LIBRARY_PATH"));
                ossetenv("LD_LIBRARY_PATH",home^"/lib:"^libp);


                //enable core dumps
                osshell("ulimit -c unlimited");

                if (verbose) {
                        osgetenv("HOME").outputl("HOME=");
                        osgetenv("PATH").outputl("PATH=");
                        osgetenv("LD_LIBRARY_PATH").outputl("LD_LIBRARY_PATH=");
                }

                //execute command or enter exodus shell
                //if (not osshell("env PS1='exodus [\\u@\\h \\W]\\$ '  "^(command?command:shell)))

                osshell(command?command:shell);

		//windows
        } else if (SLASH eq "\\" and shell.osgetenv("ComSpec")) {

                //set EXO_PATH used by compile to find LIB and INCLUDE paths
                var exoduspath=exodusbinpath;
                if (exoduspath.field2(SLASH,-1) eq "bin")
                        exoduspath=field(exoduspath,SLASH,1,dcount(exoduspath,SLASH)-1);
                if (exoduspath and not ossetenv("EXO_PATH",exoduspath))
                        errput("Couldnt set EXO_PATH environment variable");

				var oldpath=osgetenv("PATH");
				var newpath=oldpath;

                //forcibly PREFIX exodus bin to path
				//when debugging in VS ... we need to overcome any standard exodus path!
				//if (not newpath.locateusing(exodusbinpath,";"))
					newpath=exodusbinpath^";"^newpath;

				//APPEND user's Exodus binaries path (from compile/catalog)
				//could use LOCALAPPDATA on WIN7 up but getting too complicated so lets use USERPROFILE everywhere
				var homedir=osgetenv("USERPROFILE");
				if (homedir) {
					var bindir=homedir^"\\Exodus\\bin";
					if (not newpath.locateusing(bindir,";"))
						newpath^=";"^bindir;
				}

				//update path
                if (newpath ne oldpath and not ossetenv("PATH",newpath))
                        errput("Couldnt set PATH environment variable");

                //set INCLUDE path
                //prefix path to exodus.h to INCLUDE environment variable
                //1. ..\include folder parallel to path of executable
                //2. build source folder of exodus
                //3. EXO_INCLUDE envvar
                var exodusincludepath=exodusbinpath^"\\..\\include";
				var searched=exodusincludepath;
				if (!osdir(exodusincludepath)) {
					exodusincludepath=exodusbinpath^"\\..\\exodus\\exodus";
					searched.r(-1,exodusincludepath);
				}
				if (!osdir(exodusincludepath)) {
					exodusincludepath=exodusbinpath^"\\..\\..\\exodus\\exodus";
					searched.r(-1,exodusincludepath);
				}
                if (!osfile(exodusincludepath^"\\exodus\\exodus.h")) {
                        exodusincludepath=osgetenv("EXO_INCLUDE");
						searched.r(-1,exodusincludepath);
                        if (!osfile(exodusincludepath^"\\exodus.h")) {
                                errputl("Couldnt find exodus include path (exodus.h)");
								errputl(searched.swap(FM,"\n"));
                                exodusincludepath="";
                        }
                }
                if (not ossetenv("EXO_INCLUDE",exodusincludepath))
                        errput("Couldnt set EXODUS_INCLUDE environment variable");
                if (exodusincludepath and not ossetenv("INCLUDE",(exodusincludepath^";"^osgetenv("INCLUDE"))))
                        errput("Couldnt set INCLUDE environment variable");

                //set LIB path
                //prefix path to exodus.lib to LIB environment variable
                //1. path of executable
                //2. ..\lib folder parallel to path of executable
                //3. EXO_LIBPATH envvar
                var exoduslibpath=exodusbinpath;
                if (!osfile(exoduslibpath^"\\exodus.lib"))
                        exoduslibpath=exodusbinpath^"\\..\\lib";
                if (!osfile(exoduslibpath^"\\exodus.lib"))
                        exoduslibpath=osgetenv("EXO_LIBPATH");
                if (!osfile(exoduslibpath^"\\exodus.lib")) {
                        exoduslibpath=osgetenv("EXO_LIBPATH");
                        if (!osfile(exoduslibpath^"\\exodus.lib")) {
                                errput("Couldnt find exodus include path (exodus.h)");
                                exoduslibpath="";
                        }
                }
                if (not ossetenv("EXO_LIBPATH",exoduslibpath))
                        errput("Couldnt set EXO_LIBPATH environment variable");
                if (exoduslibpath and not ossetenv("LIB",(exoduslibpath^";"^osgetenv("LIB"))))
                        errput("Couldnt set LIB environment variable");

                osshell((command?command:shell));
        } else
                stop("Cannot find SHELL or ComSpec in environment");

}
