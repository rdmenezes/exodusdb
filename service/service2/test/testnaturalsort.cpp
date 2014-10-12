#include <exodus/program.h>
#include <assert.h>
programinit()

function main() {
	printl("testnaturalsort says 'Hello World!'");

	var tempfilename="tempsortfile";
	tempfilename.outputl("tempfilename=");

	var dicttempfilename="dict_"^tempfilename;
	dicttempfilename.outputl("dicttempfilename=");

	deletefile(tempfilename);
	deletefile(dicttempfilename);

	//let everything be one transaction
	//with automatic rollback at close of program ^_^
	begintrans();

	if (not createfile(tempfilename))
		abort("cant create "^tempfilename);
	if (not createfile(dicttempfilename))
		abort("cant create "^dicttempfilename);
	clearfile(tempfilename);
	clearfile(dicttempfilename);

	var tempfile;
	if (not open(tempfilename,tempfile))
		abort("cannot open "^tempfilename);

	if (not write(
	"F" _FM_
	"0" _FM_
	"ID" _FM_
	_FM_
	_FM_
	_FM_
	_FM_
	_FM_
	"R" _FM_
	"20", dicttempfilename, "ID"))
		abort("cannot write "^dicttempfilename^" ID");

	//var ndigits=9;
	//var nzeros=25;
	var ndigits=3;
	var nzeros=3;

	//create records backwards to test sort
	var ii,jj;
	for (ii=ndigits;ii>=0;--ii)
		for (jj=nzeros;jj>=0;--jj) {
			var key=""^ii^str("0",jj);
			write("x",tempfile,key.outputl("key="));
		}
	//read sorted records forwards to check numbers
	if (not tempfile.select("by IDxxx"))
		abort("problem sorting file");
	var key;
	//while (tempfile.readnext(key)) {
	while (tempfile.readnext(key)) {
		printl(key);
	}

	//rely on automatic rollback
	//deletefile(tempfilename);
	committrans();

	return 0;
}

programexit()

