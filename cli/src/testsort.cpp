#include <exodus/exodus.h>

//for the sake of multivalue gurus new to exodus/C++ this is written
//with multivalue-mimicking "everything is a global function" syntax
//instead of exodus's native OO-style "objects and methods" syntax

var filename="myclients";

subroutine sortselect(in file, in sortselectcmd)
{

	println("\nSSELECT the data - ", sortselectcmd);

	if (!select(sortselectcmd)) {
		println("Cannot sselect");
		stop();
	}

	println("Read the data");

	var record;
	var key;
	
	//could also use the readnextrecord() function here
	// if we had used the new selectrecord() function above
	while (readnext(key)) {

		if (not read(record, file, key)) {
			println(key, " missing from file");
			continue;
		}
		print(key, ": ");
		println(convert(record, FM, "|"));
		
	}

	return;

}

program()
{

	if (not connect()) {
		println("Cannot connect to database. Please check configuration");
		stop();
	}

	var dictfilename="dict_"^ filename;

	//leave the test data files around for playing with
	var cleanup=false;
	if (cleanup) {
		deletefile(filename);
		deletefile(dictfilename);
	}
	
	println("\nOpen or create test file ", filename);

	var file;
	if (not open(filename, file)) {
		createfile(filename);
		if (not open(filename, file)) {
			println("Cannot open ", filename);
			stop();
		}
	}

	println("\nOpen or create the test files dictionary ", dictfilename);

	var dictfile;
	if (not open(dictfilename, dictfile)) {
		createfile(dictfilename);
		if (not open(dictfilename, file)) {
			println("Cannot open dictionary ", dictfilename);
			stop();
		}
	}

	println("\nPrepare some dictionary records");

	var dictrecs = "";
	dictrecs  =      "CLIENT_CODE |F|0|Code     ||||          ||L|8";
	dictrecs ^= FM ^ "CLIENT_NAME |F|1|Name     ||||          ||T|15";
	dictrecs ^= FM ^ "CLIENT_TYPE |F|2|Type     ||||          ||L|5";
	dictrecs ^= FM ^ "DATE_CREATED|F|3|Date     ||||D4        ||L|12";
	dictrecs ^= FM ^ "TIME_CREATED|F|4|Time     ||||MTH       ||L|12";
	dictrecs ^= FM ^ "BALANCE     |F|5|Balance  ||||MD20P     ||R|10";
	dictrecs ^= FM ^ "TIMESTAMP   |F|6|Timestamp||||[DATETIME]||L|12";
	dictrecs ^= FM ^ "@CRT        |G| |CLIENT_CODE CLIENT_NAME CLIENT_TYPE BALANCE DATE_CREATED TIME_CREATED TIMESTAMP";

	println("\nWrite the dictionary records to the dictionary");

	var nrecs=count(dictrecs, FM)+1;
	for (var recn = 1; recn <= nrecs; recn++)
	{
		var dictrec=extract(dictrecs, recn);
		var key=field(dictrec, "|", 1);
		var rec=field(dictrec, "|", 2, 9999);
		
		println(key, ": ", rec);
		key=trim(key);
		rec=trim(rec);
		rec=swap(rec, " |", "|");
		rec=convert(rec, "|", FM);
		
		write(rec, dictfile, key);
		
		//check we can read the record back
		var rec2;
		dictfile.outputln("dictfile");
		key.outputln("key");
		if (read(rec2,dictfile, key)) {
			if (rec2 ne rec) {
				println("record differs?!");
			}
		} else
			println("Cant read ", key, " back");
	}

	var rec;
	if (not read(rec,dictfile,"BALANCE"))
		println("Cant read BALANCE record from dictionary");

	println("\nClear the client file");
	clearfile(filename);

	println("\nPrepare some data records in a readable format");

	var recs = "";
	recs ^= FM ^ "SB001|Client AAA |A |15070|76539|1000.00|15070.76539";
	recs ^= FM ^ "JB002|Client BBB |B |15000|50539|200.00|15000.50539";
	recs ^= FM ^ "JB001|Client CCC |B |15010|60539|2000.00|15010.60539";
	recs ^= FM ^ "SB1  |Client SB1 |1 |     |     |       |           ";
	recs ^= FM ^ "JB2  |Client JB2 |2 |14000|10539|0      |14000.10539";
	recs ^= FM ^ "JB10 |Client JB10|10|14010|10539|2000.00|14010.10539";
	splicer(recs, 1, 1, "");

	println("\nWrite the data records to the data file");

	nrecs=count(recs, FM)+1;
	for (var recn = 1; recn <= nrecs; recn++)
	{
		var rec=extract(recs, recn);
		var key=field(rec, "|", 1);
		rec=field(rec, "|", 2, 9999);
		println(key, ": ", rec);
		while (index(rec, " |"))
			swapper(rec, " |", "|");
		write(trimb(convert(rec, "|", FM)), file, trim(key));
	}

	var prefix="SELECT "^ filename;
	
	gosub sortselect(file, prefix^ " BY CLIENT_CODE");

	gosub sortselect(file, prefix^ " BY BALANCE BY CLIENT_CODE");

	gosub sortselect(file, prefix^ " BY TIMESTAMP");

	gosub sortselect(file, prefix^ " WITH CLIENT_TYPE 'B' BY BALANCE");

	var cmd="list "^ filename^ " id-supp";
	println("\nList the file using ", quote(cmd));
	osshell(cmd);
	
	cmd="list "^ dictfilename^ " id-supp";
	println("\nList the dict using ", quote(cmd));
	osshell(cmd);
	
	if (cleanup) {
		println("\nCleaning up. Delete the files");
		deletefile(file);
		deletefile(dictfile);
	}
	
	println("\nJust type 'list' (without the quotes) to see it's syntax");
	println("or list dict_"^ filename^ " to see the dictionary");
}