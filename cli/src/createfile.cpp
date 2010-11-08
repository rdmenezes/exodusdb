#include <exodus/exodus.h>

program()
{

        var filenames=COMMAND.field(FM,2,999999);
        var silent=index(ucase(OPTIONS),"S");

        if (not filenames)
                stop("Syntax is createfile filename filename ...\ndict files are named dict_filename");

        var nfiles=dcount(filenames,FM);
        for (var filen=1; filen<=nfiles; ++filen) {

                var filename=filenames.extract(filen);

                if (createfile(filename)) {
                        if (not silent)
                                print("Created ");
                } else {
                        if (not silent)
                                print("Cannot create ");
                }
                if (not silent)
                        printl(filename);

        }
}





