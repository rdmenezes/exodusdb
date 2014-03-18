#include <exodus/library.h>
libraryinit()

#include <listen2.h>
#include <holiday.h>
#include <lertroutine.h>
#include <authorised.h>
#include <raise.h>
#include <sysmsg.h>
#include <elapsedtimetext.h>
#include <sendmail.h>
#include <convert.h>
#include <listen4.h>

#include <gen.h>
#include <win.h>

var SENTENCE;
var suppressemail;//num
var docids;
var docn;//num
var docid;
var xx;
var toaddress;
var holidaytype;
var workdate;
var authtasks;
var title;
var DATA;
var fromtime;
var daysago;//num
var xdate;//num
var opendate;//num
var printfilename;
var body;
var attachfilename;
var errormsg;
var tracing;//num
var linkfilename2;
var requeststarttime;//num
var xx!!!;
var requeststoptime;//num
var printfile;

function main() {
	//LISTEN calls this every minute
	//print 'autorun'

	if (SENTENCE.field(" ", 1) == "AUTORUN") {
		SENTENCE = "";
		call autorun();
		var().stop();
	}

	var().clearcommon();

	//used to email NEOSYS if NEOSYS user in list of recipients
	var sysmsgatneosys = "sysmsg@neosys.com";

	//used to email if run as NEOSYS unless document's forced email address is set
	var devatneosys = "dev@neosys.com";

	//command mode forces, call mode only every 10 seconds
	var commandline = SENTENCE.field(" ", 1) == "AUTORUN";
	//logging=@username='NEOSYS' or dir('NEOSYS.ID')
	var logging = 0;

	//command line is only used for testing at the moment
	//LISTEN does "call autorun" in order to retain output on screen but a crash
	//in autorun itself then causes LISTEN to fail - causing a reset/restart
	if (commandline) {
		var options = SENTENCE.field("(", 2);
		SENTENCE = SENTENCE.field("(", 1);
		suppressemail = options.index("S", 1);

		//if not called with X option then reexecute with X option
		//so that after crash the original user and station can be restored
		if (not(options.index("X", 1))) {
			var origuser = USERNAME;
			var origstation = STATION;
			var origbatchmode = SYSTEM.a(33);

			(SENTENCE ^ " (X" ^ options).execute();

			//resume being the original user and station
			SYSTEM.r(33, origbatchmode);
			var connection = "VERSION 3";
			connection.r(2, origstation);
			connection.r(3, origstation);
			call listen2("BECOMEUSERANDCONNECTION", origuser, "", connection);
			var().stop();
		}

	}else{
		suppressemail = 0;
	}

	var users;
	if (not(users.open("USERS", ""))) {
		call fsmsg();
		return 0;
	}

	var markets;
	if (not(markets.open("MARKETS", ""))) {
		if (not(ACCOUNT == "ACCOUNTS")) {
			call fsmsg();
		}
		markets = "";
		return 0;
	}

	var datasetcode = SYSTEM.a(17);
	var islivedb = not SYSTEM.a(61);

	//allow one autorun per database - hopefully this wont overload the server
	var lockfilename = "DOCUMENTS";
	var lockfile = gen.documents;
	var lockkey = "%" ^ datasetcode ^ "%";
	if (not(lockrecord(lockfilename, lockfile, lockkey, "", 1))) {
		return 0;
	}

	//path to output reports
	var webpath = "";
	if (not webpath) {
		webpath = "..\\" "DATA\\";
	}
	if (webpath.substr(-1, 1) ne "\\") {
		webpath ^= "\\";
	}
	var inpath = webpath ^ datasetcode ^ "\\";

initdoc:
	////////
	if (commandline) {
		docids = SENTENCE.field(" ", 2);
	}else{
		docids = "";
	}
	if (docids) {
		docids.swapper(",", FM);
		docn = 0;
	}else{
		gen.documents.select();
	}
	var locked = 0;
	var ndocsprocessed = 0;

nextdoc:
	////////

	if (locked) {
		call unlockrecord("DOCUMENTS", gen.documents, docid);
		locked = 0;
	}

	//dont process all documents one after the other within one
	//call of autorun to avoid overloading the subroutine stack cache etc
	//so docexit doesnt goto nextdoc .. it goes to exit
	if (ndocsprocessed > 1) {
		goto exit;
	}

	if (docids) {
		docn += 1;
		docid = docids.a(docn);
		if (not docid) {
			goto exit;
		}
	}else{
		if (not docid.readnext()) {
			goto exit;
		}
	}

readdoc:
	////////
	//to save processing time, dont lock initially until looks like it needs processing
	//then lock/read/check again in case other processes doing the same
	//depending on the autorunkey then this may be redundant

	//get document
	if (not(gen.document.read(gen.documents, docid))) {
		if (commandline) {
			call mssg(DQ ^ (docid ^ DQ) ^ " alert doesnt exist in AUTORUN");
		}
		goto nextdoc;
	}

	//only do saved and enabled documents for now
	//0/1 saved disabled/enabled. Blank=Ordinary documents
	if (not gen.document.a(12)) {
		goto nextdoc;
	}

	if (logging) {
		cout << docid;
		cout << " ";
	}

	//determine current datetime
currdatetime:
	/////////////
	var itime = var().time();
	var idate = var().date();
	//handle rare case where passes midnight between time() and date()
	if (var().time() < itime) {
		goto currdatetime;
	}
	var currdatetime = idate + itime / 86400;

	//skip if scanning all docs and not time to process yet
	//allow processing individual docs manually despite timing/scheduling
	if (docids == "") {

		//would be faster to work out nextdatetime once initially - but how to do it?
		//if not(docids) and currdatetime<nextdatetime then goto nextdoc

		var lastdatetime = gen.document.a(13);

		//skip if already run in the last 60 minutes. this is an easy way
		//to avoid reruns but maximum scheduling frequency is hourly
		if ((currdatetime - lastdatetime).abs() <= 1 / 24) {
			goto nextdoc;
		}

		//scheduling in document all can be multivalued
		//21 min 0-59 (not used currently)
		//22 hour 0-23 (minimum hour if only one)
		//23 day of month 1-31 means 1st-31st
		//24 month of year 1-12 means Jan-Dec
		//25 day of week 1-7 means Mon-Sun
		//26 date
		//27 max number of times

		var restrictions = trim2(gen.document.field(FM, 21, 7), FM, "B");
		restrictions.converter(",", VM);

		//skip if no restrictions applied yet
		if (restrictions == "") {
			goto nextdoc;
		}

		//only run scheduled reports on live data (but do run queued reports)
		//queued reports have only maxtimes=1 set
		if (restrictions ne (FM.str(6) ^ 1) and not islivedb and not var("NEOSYS.ID").osfile()) {
			if (logging) {
				cout << "scheduled report but not live db" << endl;
			}
			goto nextdoc;
		}

		//hour of day restrictions
		var hours = restrictions.a(2);
		if (hours ne "") {
			var hournow = (itime.oconv("MT")).substr(1, 2) + 0;

			//if one hour then treat it as a minimum hour
			if (hours.isnum()) {
				if (hournow < hours % 24) {
					if (logging) {
						cout << "not yet hour" << endl;
					}
					goto nextdoc;
				}else{
					//ensure not done already today in a previous hour
					goto preventsameday;
				}

				//or specific multiple hours
			}else{
				if (not(hours.locateusing(hournow, VM, xx))) {
					if (logging) {
						cout << "wrong hour" << endl;
					}
					goto nextdoc;
				}
			}

			//if no hourly restrictions then skip if already run today
		}else{
preventsameday:
			if (currdatetime.floor() == lastdatetime.floor()) {
				if (logging) {
					cout << "already run today" << endl;
				}
				goto nextdoc;
			}
		}

		var date = idate.oconv("D/E");

		//day of month restrictions
		if (restrictions.a(3)) {
			if (not(restrictions.a(3).locateusing(date.field("/", 1) + 0, VM, xx))) {
				if (logging) {
					cout << "wrong day of month" << endl;
				}
				goto nextdoc;
			}
		}

		//month of year restrictions
		if (restrictions.a(4)) {
			if (not(restrictions.a(4).locateusing(date.field("/", 2) + 0, VM, xx))) {
				if (logging) {
					cout << "wrong month" << endl;
				}
				goto nextdoc;
			}
		}

		//day of week restrictions
		if (restrictions.a(5)) {
			if (not(restrictions.a(5).locateusing(idate - 1 % 7 + 1, VM, xx))) {
				if (logging) {
					cout << "wrong day of week" << endl;
				}
				goto nextdoc;
			}
		}

		//date restrictions
		if (restrictions.a(6)) {
			if (not(restrictions.a(6).locateusing(idate, VM, xx))) {
				if (logging) {
					cout << "wrong date" << endl;
				}
				goto nextdoc;
			}
		}

		//would be better to work out next run time once - but how to work it out
		//if not(docids) and currdatetime<nextdatetime then goto nextdoc

	}

	//lock documents that need processing
	//but reread after lock in case another process has not started processing it
	if (not locked) {
		if (not(lockrecord("DOCUMENTS", gen.documents, docid))) {
			if (logging) {
				cout << "locked" << endl;
			}
			goto nextdoc;
		}
		locked = 1;
		goto readdoc;
	}

	//register that the document has been processed
	//even if nobody present to be emailed
	gen.document.r(13, currdatetime);
	if (gen.document.a(27) ne "") {
		gen.document.r(27, gen.document.a(27) - 1);
	}
	if (gen.document.a(27) ne "" and not gen.document.a(27)) {
		gen.documents.deleterecord(docid);
	}else{
		gen.document.write(gen.documents, docid);
	}

	//force all emails to be routed to test address
	//if on development system they are ALWAYS routed
	//so this is mainly for testing on client systems
	var forceemail = gen.document.a(30);
	//if not(forceemail) and @username='NEOSYS' then forceemail=devATneosys
	//report is always run as the document owning user
	var runasusercode = gen.document.a(1);
	var USER;
	if (not(USER.read(users, runasusercode))) {
		if (not(runasusercode == "NEOSYS")) {
			cout << "runas user ";
			cout << runasusercode;
			cout << " doesnt exist" << endl;
			goto nextdoc;
		}
		USER = "";
	}
	//allow running as NEOSYS and emailing to sysmsg@neosys.com
	if (USER.a(7) == "" and runasusercode == "NEOSYS") {
		USER = "NEOSYS";
		USER.r(7, sysmsgatneosys);
	}

	//HAS RECIPIENTS
	//if there are any recipients then
	//build email addresses of all recipients
	//depending on weekends/personal holidays etc
	//and skip if no recipients
	//recipients may get nothing if it is a
	//dynamically emailed report
	//they may get just summary report

	//HASNT RECIPIENTS
	//dynamically emailed reports need no recipents
	//The runasuser (if have emailaddress) will get
	//any output regardless of if they are on holiday

	var ccaddress = "";
	var usercodes = gen.document.a(14);
	if (usercodes == "") {
		toaddress = USER.a(7);
	}else{
		toaddress = "";
		var nusers = usercodes.count(VM) + 1;
		var backwards = 1;
		for (var usern = nusers; usern >= 1; --usern) {

			//get the user record
			var usercode = usercodes.a(1, usern);
			if (not(USER.read(users, usercode))) {
				if (not(usercode == "NEOSYS")) {
					goto nextdoc;
				}
				USER = "NEOSYS";
			}

			//skip if user has no email address
			if (USER.a(7) == "" and usercode == "NEOSYS") {
				USER.r(7, sysmsgatneosys);
			}
			var useraddress = USER.a(7);
			if (useraddress) {

				//if running as NEOSYS always add user NEOSYS
				//regardless of holidays - to allow testing on weekends etc
				//if usercode='NEOSYS' then
				if (USERNAME == "NEOSYS" and usercode == "NEOSYS") {
					goto adduseraddress;

					//optionally skip people on holiday (even NEOSYS unless running as NEOSYS)
				}else{

					var marketcode = USER.a(25);
					if (not marketcode) {
						marketcode = gen.company.a(30, 1);
					}
					var market = marketcode;
					if (markets) {
						if (not(market.read(markets, marketcode))) {
							{}
						}
					}

					idate = var().date();
					var agp = "";
					call holiday("GETTYPE", idate, usercode, USER, marketcode, market, agp, holidaytype, workdate);

					if (not holidaytype) {
adduseraddress:
						if (backwards) {
							toaddress.inserter(1, useraddress);
						}else{
							toaddress.r(-1, useraddress);
						}
					}
				}

			}

		};//usern;

		//skip if nobody to email to
		if (not toaddress) {
			if (logging) {
				cout << "nobody to email" << endl;
			}
			goto nextdoc;
		}

		toaddress.converter(FM, ";");

	}

	//before running the document refresh the title, request and data
	var module = gen.document.a(31);
	var alerttype = gen.document.a(32);

	if (module and alerttype) {

		var alertroutine = module ^ ".ALERTS";
		call lertroutine(alerttype, runasusercode, authtasks, title, USER0, DATA);

		//update the document and documents file if necessary
		call cropper(gen.document);
		var origdocument = gen.document;

		gen.document.r(2, title);
		gen.document.r(5, lower(module ^ "PROXY" ^ FM ^ USER0));
		gen.document.r(6, lower(DATA));

		call cropper(DATA);
		if (gen.document ne origdocument) {
			gen.document.write(gen.documents, docid);
		}

	}else{
		authtasks = "";
	}

	//check if runasuser is authorised to run the task
	if (authtasks) {
		var ntasks = authtasks.count(VM) + 1;
		for (var taskn = 1; taskn <= ntasks; ++taskn) {
			var task = authtasks.a(1, taskn);
			if (not(authorised(task, USER4, "", runasusercode))) {
				USER4 = DQ ^ (runasusercode ^ DQ) ^ " is not authorised to do " ^ task;
				cout << USER4 << endl;
				goto nextdoc;
			}
		};//taskn;
	}

docinit:
	////////
	if (logging) {
		cout << "running as ";
		cout << runasusercode << endl;
	}

	var fromdate = var().date();
	call dostime(fromtime);

	ndocsprocessed += 1;

	//become the user so security is relative to the document "owner"
	var connection = "VERSION 3";
	connection.r(2, "0.0.0.0");
	connection.r(3, "SERVER");
	connection.r(4, "");
	connection.r(5, "");
	call listen2("BECOMEUSERANDCONNECTION", runasusercode, "", connection);

	//request='EXECUTE':fm:'GENERAL':fm:'GETREPORT':fm:docid
	//voccmd='GENERALPROXY'
	USER0 = raise("EXECUTE" ^ VM ^ gen.document.a(5));

	USER1 = raise(gen.document.a(6));

	//override the saved period with a current period

	//get today's period
	var runtimeperiod = ((var().date()).oconv("D2/E")).substr(4, 5);
	if (runtimeperiod.substr(1, 1) == "0") {
		runtimeperiod.splicer(1, 1, "");
	}
	//should backdate period to maximum open period for all selected companies
	//to avoid "year is not open" type messages
	//TODO

	USER1.swapper("{RUNTIME_PERIOD}", runtimeperiod);
	USER1.swapper("{TODAY}", var().date());
	USER1.swapper("{7DAYSAGO}", var().date() - 7);
	USER1.swapper("{14DAYSAGO}", var().date() - 14);
	USER1.swapper("{21DAYSAGO}", var().date() - 21);
	USER1.swapper("{28DAYSAGO}", var().date() - 28);
	USER1.swapper("{30DAYSAGO}", var().date() - 30);
	USER1.swapper("{60DAYSAGO}", var().date() - 60);
	USER1.swapper("{90DAYSAGO}", var().date() - 90);
	USER1.swapper("{YESTERDAY}", var().date() - 1);
	USER1.swapper("{TOMORROW}", var().date() + 1);
	if (USER1.index("{2WORKINGDAYSAGO}", 1)) {
		daysago = 2;
		gosub getdaysago();
		USER1.swapper("{2WORKINGDAYSAGO}", xdate);
	}
	if (USER1.index("{3WORKINGDAYSAGO}", 1)) {
		daysago = 3;
		gosub getdaysago();
		USER1.swapper("{3WORKINGDAYSAGO}", xdate);
	}
	var closedperiod = gen.company.a(37);
	if (closedperiod) {
		opendate = closedperiod.iconv(gen.company.a(6)) + 1;
	}else{
		opendate = 11689;
	}
	USER1.swapper("{OPERATIONS_OPEN_DATE}", opendate);

	//convert {TODAY-99} to today minus 99
	//and {TODAY+999} to today+999
	var sign = "-";
nextsign:
	var tt = USER1.index("{TODAY" ^ sign, 1);
	if (tt) {
		var t2 = USER1.substr(tt + 1, 999999).field("}", 1);
		USER1.swapper("{" ^ t2 ^ "}", var().date() + t2.substr(6, 999999));
	}
	if (sign == "-") {
		sign = "+";
		goto nextsign;
	}

	//run the report
	gosub execute();

docexit:
	////////

	if (not suppressemail) {

		//email only if there is an outputfile
		//ANALTIME2 emails everything out and returns 'OK ... ' in response
		//if dir(printfilename)<1> else goto nextdoc

		var subject = "NEOSYS";
		//if repeatable then include report number to allow filtering
		if (gen.document.a(27) == "") {
			subject ^= " " ^ docid;
		}
		subject ^= ": %RESULT%" ^ gen.document.a(2);

		//email it
		if (USER3.substr(1, 2) ne "OK" or printfilename.osfile().a(1) < 10) {

			//plain "OK" with no file means nothing to email
			if (USER3 == "OK") {
				goto nextdoc;
			}

			body = "";
			body.r(-1, USER3);
			if (USER3.substr(1, 6) == "Error:") {
				USER3.splicer(1, 6, "Result:");
			}
			if (USER3.index("Error", 1)) {
				subject ^= " ERROR";
				var(USER3).oswrite("xyz.xyz");
			}
			//swap 'Error:' with 'Result:' in body
			body.r(-1, var("Document: " ^ gen.document.a(2) ^ " (" ^ docid ^ ")").trim());
			body.r(-1, "Database: " ^ SYSTEM.a(23) ^ " (" ^ SYSTEM.a(17) ^ ")");
			//swap '%RESULT%' with '* ' in subject
			subject.swapper("%RESULT%", "");

			//treat all errors as system errors for now
			//since autorun doesnt really know a user to send them to
			//NB programs should return OK+message if no report is required (eg "OK no ads found")
			if (USER3.substr(1, 2) == "OK") {
				USER3 = USER3.substr(3, 999999).trimf();
			}else{
				call sysmsg(subject ^ FM ^ body);
				goto nextdoc;
			}

			attachfilename = "";

		}else{

			var timetext = elapsedtimetext(fromdate, fromtime);

			//if ucase(printfilename[-4,4])='.XLS' then
			//locate ucase(field2(printfilename,'.',-1)) in 'XLS,CSV' using ',' setting xx then
			tt = field2(printfilename.ucase(".", -1));
			if (tt.index("HTM", 1) and gen.document.a(33) ne "2") {
				//insert body from file
				body = "@" ^ printfilename;
				subject ^= " in " ^ timetext;
			}else{
				attachfilename = printfilename;
				body = timetext;
			}

			subject.swapper("%RESULT%", "");
		}
		body.swapper(FM, var().chr(13));

		SYSTEM.r(117, forceemail);
		cout << "Emailing ";
		cout << toaddress<< endl;
		cout << " ...";
		call sendmail(toaddress, ccaddress, subject, body, attachfilename, "", errormsg);
		SYSTEM.r(117, "");
		if (errormsg and errormsg ne "OK") {
			//call msg(errormsg)
			call sysmsg(errormsg);
			cout << errormsg << endl;
		}

	}

	if (tracing) {
		cout << var("�").str(79) << endl;
	}
	goto nextdoc;

}

subroutine execute() {

	var voccmd = USER0.a(2);

	tracing = 1;

	//generate a unique random output file
	while (true) {
		linkfilename2 = inpath ^ (((var(10).pwr(15)).rnd()).str(8)).substr(1, 8);
		(linkfilename2 ^ ".*").initdir();
	///BREAK;
	if (not var().oslistf()) break;;
	}//loop;
	linkfilename2 ^= ".2";
	call oswrite("", linkfilename2);

	//turn interactive off in case running from command line
	//to avoid any reports prompting for input here
	var s33 = SYSTEM.a(33);
	SYSTEM.r(33, 1);

	gosub execute2();

	//restore interactivity status
	SYSTEM.r(33, s33);

	return;

}

subroutine execute2() {
	call dostime(requeststarttime);
	//system<25>=requeststarttime
	//allow autorun processes to run for ever
	SYSTEM.r(25, "");
	USER0 = USER0.field(FM, 3, 99999);

	var localtime = var().time() + SW.a(1) % 86400;
	cout << var().cursor(0)<< " "<< USERNAME<< " "<< FM.convert(" ", USER0)<< " "<< gen.document.a(2)<< endl;
	cout << var().cursor(-4);
	cout << localtime.oconv("MTS");
	cout << " AUTORUN ";
	cout << docid;

	//print 'link',linkfilename2
	//print 'request',request
	//print 'iodat',iodat

	//following simulates LISTEN's 'EXECUTE'

	//provide an output file for the program to be executed
	//NB response file name for detaching processes
	//will be obtained from the output file name LISTEN2 RESPOND
	//this could be improved to work
	printfilename = linkfilename2;
	var tt!!! = var().osdir();
	(tt!!!).splicer(-7, 7, "");
	if (printfilename.substr(1, tt!!!).length() == tt!!!) {
		printfilename.splicer(1, tt!!!).length(, "..\\");
	}

	//t=printfilename[-1,'B.']
	tt!!! = field2(printfilename, ".", -1);
	printfilename.splicer(-(tt!!!).length(), tt!!!).length(, "htm");
	SYSTEM.r(2, printfilename);
	if (tracing) {
		cout << datasetcode;
		cout << " Waiting for output ... ";
	}

	//execute the program
	//request, iodat and response are now passed and returned in @user0,1 and 3
	//other messages are passed back in @user4
	//execute instead of call prevents program crashes from crashing LISTEN
	USER3 = "OK";
	win.valid = 1;
	USER4 = "";

	//pass the output file in linkfilename2
	//not good method, pass in system?
	if (var("LIST" _VM_ "SELECTBATCHES").a(1).locateusing(USER0.a(1), VM, xx!!!)) {
		USER1 = linkfilename2;
	}

	SYSTEM.r(117, forceemail);

	voccmd.execute();

	SYSTEM.r(117, "");

	//discard any stored input
	DATA = "";

	//detect memory corruption?
	//@user4='R18.6'
	if (USER4.index("R18.6", 1)) {
		var halt = 1;
		USER4.r(-1, "Corrupt temporary file. Restart Needed.");
		USER4.r(-1, "NEOSYS.NET TERMINATED");
	}

	//convert error message
	if (USER4.index(" IN INDEX.REDUCER AT ", 1) or USER4.index(" IN RTP21 AT ", 1)) {
		//@user4='Please select fewer records and/or simplify your request'
		call listen4(17, USER4);
	}

	//no records are not system errors
	if (USER3.substr(1, 2) == "OK") {
		goto 3871;
	}
	if (USER3.substr(1, 9) == "No record") {
norecords:
		USER3.splicer(1, 0, "OK ");
		USER4 = "";
		goto 3871;
	}
	if (USER3.substr(1, 7) == "No item") {
		goto norecords;
	}
L3871:

	//send errors to neosys
	if (USER4.index("An internal error", 1) or USER4.index("Error:", 1)) {
		USER4.transfer(USER3);
		goto sysmsgit;
	}

	//send errors to neosys
	if (USER3 == "" or USER3.substr(1, 2) ne "OK") {
		if (not USER3) {
			USER3 = "No response from " ^ voccmd;
		}
sysmsgit:
		call sysmsg("AUTORUN " ^ docid ^ " " ^ gen.document.a(2) ^ FM ^ USER3);
	}

	call cropper(USER4);
	call cropper(USER3);

	if (USER4) {
		USER1 = "";
		USER3 = "Error: " ^ USER4;
		gosub fmtresp();
	}

	if (USER3 == "") {
		//response='Error: No OK from ':voccmd:' ':request
		call listen4(18, USER3, voccmd);
		gosub fmtresp();
	}

	var rawresponse = USER3;
	rawresponse.converter("\r\n", "|");

	//get the printfilename in case the print program changed it
	printfilename = SYSTEM.a(2);
	//and close it in case print program didnt (try to avoid sendmail attach errors)
	printfilename.osclose();
	if (tt.osopen(printfilename)) {
		tt.osclose();
	}
	var().osflush();

	//trace responded
	call dostime(requeststoptime);
	if (tracing) {
		cout << var().cursor(0)<< rawresponse << endl;
		cout << var().cursor(-4);
		cout << "Responded in ";
		cout << (requeststoptime - requeststarttime).oconv("MD20P");
		cout << " SECS ";
		//do after "emailing" message
		//print str('�',79)
		//print linkfilename1
	}

	//make sure that the output file is closed
	if (printfile.osopen(printfilename)) {
		printfile.osclose();
	}

	return;

	/////
exit:
	/////
	call unlockrecord(lockfilename, lockfile, lockkey);

	return;

}

subroutine fmtresp() {

	//trim everything after <ESC> (why?)
	tt!!! = USER3.index("<ESC>", 1);
	if (tt!!!) {
		USER3 = USER3.substr(1, tt!!! - 1);
	}

	//cannot remove since these may be proper codepage letters
	//convert '��' to '' in response
	USER3.converter("|", FM);
	USER3.converter(VM, FM);
	if (USER3.substr(1, 1) == FM) {
		USER3.splicer(1, 1, "");
	}
	USER3.swapper(FM, "\r\n");

	return;

}

subroutine getdaysago() {
	var weekend = "67";
	var marketcode = gen.company.a(30, 1);
	if (marketcode) {
		tt = marketcode.xlate("MARKETS", 9, "X");
		if (tt) {
			weekend = tt;
		}
	}

	xdate = var().date();
	while (true) {
		xdate -= 1;
		if (not(weekend.index(xdate - 1 % 7 + 1, 1))) {
			daysago -= 1;
		}
	///BREAK;
	if (not daysago) break;;
	}//loop;

	return;

}


libraryexit()