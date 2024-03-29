/* Copyright (c) 2007 Stephen John Bush - see LICENCE.TXT*/
//#include <iostream>
//#include "server.h"

//#include "mvroutine.h"

//TODO replace wcout<< with printl to remove dependency on <iostream>

//perhaps following is not necessary
//and only need to grant www-data group access (+sgid) to data since exodus is owner
//of data therefore can delete files created and owned by www-data there
//
//needs to read and delete files written by www-data
//needs to create files readable and deletable by www-data
//therefore
//1.create a www-exodus group
//2.add www-data and exodus server to it
//3.set the data directory (where www-data and exodus communicate) to www-exodus
/*
sudo groupadd www-exodus
sudo usermod -a -G www-exodus exodus
sudo usermod -a -G www-exodus www-data
sudo chmod -R g+x /home/exodus/service #and all parent directories
sudo chmod -R g+rwxs /home/exodus/service/data
sudo chgrp -R www-exodus /home/exodus/service/data
*/
//posix permissions principles
//only root can change the user of a file
//you can only change the group to groups that you belong to
//the group of a created files can be forced to be the group of the directory
//set the sgid chmod +s
//sometimes you can rename files but not write/delete them
//you must have x privilege to all directories above in order to traverse into one
//"all directories above" means of the real directories not apparent dir if using symbolic links
//check permissions by sudo su <user> and cd/touch etc

#include <exodus/program.h>
programinit()

#include "win.h"//common
#include "select2.h"
#include "securitysubs.h"

	const var fefdfcfb = _FM_ _VM_ _SM_ _TM_;

	bool routineexists;

	var nblocks;
	var datx[4];
	var hexx[256];
	var sleepms;//num
	var origscrn;
	var origattr;
	var locks;
	var logfilename;
	var logfile;
	var logptr;//num
	var xx;
	var logx;
	var timenow;
	var linkfilenames;
	var linkfilename0;
	var charx;
	var buffer;
	var reply;
	var bakreq;
	var lastbakattemptdate;
	var bakdows;
	var minbaktime;
	var maxbaktime;
	var testdata;
	var bakdisk;
	var requeststarttime;//num
	var linkfile1;
	var linkfilename2;
	var invaliduser;
	var nostack;
	var newfilename;
	var ok;
	var createnotallowed;
	var file;
	var lockauthorised;//num
	var requeststoptime;//num
	var iodatlen;

	var request1;
	var request2;
	var request3;
	var request4;
	var request5;

	var code;
	var response;
	var responsefilename;
	var linkfilename1;
	var linkfilename3;
	var replyfilename;
	var filename;
	var keyx;
	var sessionid;
	var Serverstation;
	var Serverusername;
	var relistlocks;

	var record;
	var state;//num
	var timeouttime;//num
	var lockdurationinmins;//num
	var newsessionid;
	var connection;
	var filename2;
	var msg0;
	var positive;
	//var msg2;
	var timeoutsecs;
	var ostimex;

	var maxstringsize;
	var inblocksize;
	//inblocksize=50000
	var outblocksize;
	var Serverversion;
	var origsentence;
	var md;
	var halt;
	var timex;
	var breaktime;
	var dataset;
	var username;
	var password;
	var Serverfailure;
	var origprivilege;
	var nrequests;
	var tracing;
	var origbatchmode;

	var portno;
	var onalertsecs;

	var datasetcode;
	var neopath;
	var globalend;
	var allcols;

	var servername;
	var onserver;
	var waitfor;
	var waitsecs;
	var serverflagfilename;
	var intranet;
	var webpath;
	var inpath;
	var batchmode;
	var ostimenow;//num
	var lockduration;
	var lockkey;
	var lockx;
	//delete files older than ...
	var ageinsecs;
	var lockrec;
	var withlock;
	var updatenotallowed;
	var logpath;
	var onalert;
	var timestarted;

//labelled common
#include "win.cpp"

#define a extract

function main()
{
	//CREATE LABELLED COMMON
	mv.labelledcommon[1]=new win_common;

	nblocks = 4;

	portno = 5700;
	if (mv.SYSTEM.a(38))
		portno += mv.SYSTEM.a(38) - 1;

	onalertsecs = 0;

	//delete files older than ...
	ageinsecs = 60 * 60;

	halt = 0;

//TODO	var("ADDMFS SHADOW.MFS FILEORDER.COMMON").perform();

	maxstringsize = 65530;
	inblocksize = 65500;
	//inblocksize=50000
	outblocksize = (maxstringsize / 3).floor();

	Serverversion = "";
	origsentence = SENTENCE;

	//open 'messages' to messages else return 999999
//	mv.SYSTEM.printl();

	//openqm connection
	//std::wcout<<"Connecting DB ... "<<std::flush;
	//if (!this->SESSION.connect("127.0.0.1","4243","steve","stetempp","QMSYS"))
	//{
	//	std::wcout<<"couldn't connect to QMSYS"<<std::endl;
	//	return false;
	//}
	//std::wcout<<"OK"<<std::endl;
	var conninfo="";//"host=localhost port=5432 dbname=exodus user=exodus password=somesillysecret connect_timeout=10";
	if (!mv.SESSION.connect(conninfo))
	{
		std::wcerr<<"MvEnvironment::init: Couldn't connect to local database"<<std::endl;
		return false;
	}

	//std::wcout<<"Opening Definitions ... "<<std::flush;
	var definitionsfilename="DEFINITIONS";
	if (!mv.DEFINITIONS.open(definitionsfilename))
	{
		if (!mv.DEFINITIONS.createfile(definitionsfilename)||!mv.DEFINITIONS.open(definitionsfilename))
		{
			std::wcerr<<"Cannot create "<<definitionsfilename<<std::endl;
			return false;
	}
	}
	//std::wcout<<"OK"<<std::endl;

	/* arev's byte/character bit inverter not available for now
	//std::wcout<<"Reading Security ... "<<std::flush;
	if (!mv.SECURITY.read(mv.DEFINITIONS,"SECURITY"))
	{
		//std::wcout<<"Cannot read SECURITY"<<std::endl;
		//return false;
		mv.SECURITY="";
	}
	mv.SECURITY=mv.SECURITY.invert();
//	std::wcout<<"OK"<<std::endl;
	*/

	//std::wcout<<"Opening MD ... "<<std::flush;
	var mdfilename="MD";
	var md;
	if (!md.open(mdfilename))
	{
			if (!md.createfile(mdfilename)||!md.open(mdfilename))
		{
			std::wcerr<<"Cannot create "<<mdfilename<<std::endl;
			return false;
		}
	}
	//std::wcout<<"OK"<<std::endl;

	//std::wcout<<"Opening MENUS ... "<<std::flush;
	var menufilename="MENUS";
	var menus;
	if (!menus.open(menufilename))
	{
		if (!menus.createfile(menufilename)||!menus.open(menufilename))
		{
			std::wcerr<<"Cannot create "<<menufilename<<std::endl;
			return false;
		}
	}
	//std::wcout<<"OK"<<std::endl;

	//std::wcout<<"MvEnvironment::init: completed "<<std::endl;
	datasetcode = mv.SYSTEM.a(17);
	if (datasetcode == "")
//		datasetcode = "DEFAULT";
		datasetcode = "exodus";

#ifdef _WIN32
	neopath = "d:\\neosys\\";
#else
//	neopath = "/cygdrive/d/neosys/neosys/";
	neopath = "../";
#endif

	neopath.converter("/\\",SLASH^SLASH);
	globalend = neopath ^ "GLOBAL.END";
	if (!allcols.open("ACCESSIBLE_COLUMNS"))
		allcols = "";

	//prepare for sending and receiving escaped iodat > 64kbh
	for (int ii = 0; ii <= 255; ii++)
	{
		hexx[ii]= ("%" ^ (var(ii).oconv("MX")).oconv("R(0)#2"));
		//hexx[ii]="%00";
//		std::wcout<<hexx[ii]<<" ";
	}

	//discover the server name
	//servername=field(getdrivepath(drive()[1,2])[3,9999],'\',1)
	servername = "";
	onserver = servername == "" || mv.STATION.trim() == servername;

	//should be less than 25 unless waiting.exe modified to update the server flag
	waitfor = 1;
	waitsecs = 10;

	//if not on "local" drive c,d or e then longer sleep to save network traffic
	if (onserver) {
		sleepms = 10;
	}else{
		sleepms = 100;
	}

	//make up a serverflagfilename
	serverflagfilename = "GLOBAL.SVR";

	intranet = origsentence.index("INTRANET", 1);

	gosub getbakpars();

	//if @username='neosys.net' then system<33>=1
	origbatchmode = mv.SYSTEM.a(33);

	//webpath=field(origsentence,' ',3)
	webpath = neopath ^ "data/";
	if (!webpath)
		webpath = "../data/";
	webpath.converter("/\\",SLASH ^ SLASH);
	if (webpath.substr(-1, 1) ne SLASH)
		webpath ^= SLASH;
	if (!md.open("MD")) {
		mv.mssg("CANNOT OPEN MD FILE");
		return 999999;
	}

	inpath = webpath ^ datasetcode ^ SLASH;

	//set time before calling deleteoldfiles which is time sensitive
	//call settime(webpath[1,2])

	gosub deleteoldfiles(inpath,"^.*$");

	Serverusername = USERNAME;
	Serverstation = mv.STATION;

	//savescreen(origscrn, origattr);

//	std::wcout << mv.AW.a(30);

	//tracing=(@username='neosys' or trim(@station)='sbcp1800')
	//tracing=(not(index(origsentence,'auto',1)))

	//if @username='demo' then tracing=0
	//tracing=0

	tracing = 1;

	//ensure unique sorttempfile
	//if (!(sysvar("SET", 192, 102, "R" ^ var("0000" ^ mv.SYSTEM.a(24)).substr(-5, 5) ^ ".SFX")))
	//	{}

	nrequests = mv.SYSTEM.a(35) + 0;
	//if tracing then
	std::wcout << "NEOSYS.NET SERVICE "<< mv.SYSTEM.a(24);
	std::wcout << " STARTED "<< var().timedate() << std::endl;
	std::wcout << std::endl;

	std::wcout << "Station  : "<< mv.STATION<<std::endl;
	std::wcout << "Drive : "<< var().oscwd() << std::endl;

	std::wcout << "Server   : "<< servername<<std::endl;
	std::wcout << "Data  : "<< inpath << std::endl;

	//print 'tcp port : ':portno
	//end else
	// *xpos=20
	// *ypos=7
	// *call makewin(xpos,ypos,40,10,'',buffer)
	// *print @(xpos+2,ypos+2):'serving
	//
	// *switch into interactive mode to put up message
	// s33=system<33>
	// system<33>=''
	//
	// * call note2('neosys.net service' 'c#30':'||||||press esc and wait 10 secs to interrupt','ub')
	//
	// *switch back to silent mode
	// system<33>=s33
	//
	// print @aw<30>:
	//
	// end

	origprivilege = mv.PRIVILEGE;
	//mv.setprivilege("");

	request1 = "";

	//image=''


	if (!(mv.openfile2("LOCKS", locks, "LISTS", 1))) {
		mv.mssg("CANNOT OPEN LOCKS FILE");
		return "";
	}

	relistlocks = 1;

	timeoutsecs = mv.SYSTEM.a(33, 5);
	if (!timeoutsecs.isnum())
		timeoutsecs = "";
	//if timeoutsecs='' then timeoutsecs=20*60;*ie 20 minutes

	gosub gettimeouttime();
	timestarted = var().time();

	gosub flagserveractive();

	onalert = onalertsecs;

	//open a log file
	logfilename = "";
	logpath = neopath ^ "/LOGS/";
	logpath.converter("/\\",SLASH^SLASH);
	if (var().oslistf(logpath ^ datasetcode)=="")
	{
		if (var().oslist(logpath)=="")
			logpath.osmkdir();
		(logpath ^ datasetcode).osmkdir();
	}
	var datex = (var().date()).oconv("D2.");
	logfilename = logpath ^ datasetcode ^ SLASH ^ datex.substr(-2, 2) ^ datex.substr(1, 2) ^ datex.substr(4, 2) ^ var("00" ^ mv.SYSTEM.a(24)).substr(-2, 2);
	logfilename ^= ".xml";

	if (logfilename.osopen()) {
		logptr = logfilename.osfile().a(1);

		//osbread x from logfilename at logptr-6 length 6
		var tt=logptr-6;
		xx.osbread(logfilename, tt, 6);
		if (xx == "</Log>")
			logptr -= 6;

	}else{
		var("").oswrite(logfilename);
		if (logfilename.osopen()) {
			logptr = 0;
			logx = "<?xml version=\"1.0\"?>" ^ (var().chr(13) ^ var().chr(10));
			logx ^= "<?xml-stylesheet type=\'text/xsl\' href=\'.\\loglist.xsl\'?>" ^ (var().chr(13) ^ var().chr(10));
			//logx:="<?xml-stylesheet type='text/xsl'?>":crlf
			logx ^= "<Log";
			logx ^= " Process=" ^ mv.SYSTEM.a(24).quote();
			logx ^= " Dataset=" ^ datasetcode.quote();
			logx ^= ">";
			gosub writelogx2();

			mv.osbwritex("</Log>", logfilename, logfilename, logptr);

		}else{
			std::wcout << "CANNOT OPEN LOG FILE "<< logfilename << std::endl;
			logfilename = "";
		}
	}

	linkfilename1 = "";
	replyfilename = "";

	mv.TCLSTACK = mv.TCLSTACK.field(FM, 1, 10);
	//<arev>
//	var("RESET").execute();
	//</arev>

	if (tracing)
		std::wcout << var("-").str(79) << std::endl;
	RECORD = "";
	ID = "";
	USER0 = "";
	USER1 = "";
	USER3 = "";
	USER4 = "";
	//garbagecollect;

	//linkfilename1=''

	//force into interactive mode for deb ugging
	//if tracing then
	if (1) {
		mv.SYSTEM.r(33, 1);
	}else{
		mv.SYSTEM.r(33, "");
	}
	//end else
	// end

	//force into server mode - suppress interactive messages
	batchmode = origbatchmode;
	if (batchmode == "") {
		batchmode = mv.SYSTEM.a(33);
		mv.SYSTEM.r(33, "1");
	}
	// if trim(@station)='sb_compaq' and @username='neosys' then system<33>=''

	//make a per process request input file
	while (true)
	{
		linkfilename1 = inpath ^ var(99999999).rnd() ^ ".0";
		linkfilename1.osdelete();
		if (linkfilename1.osfile()=="")
			break;

		//if (tracing)
		std::wcout << "CANNOT DELETE "<< linkfilename1<< " GENERATING ANOTHER" << std::endl;

	}
	linkfilename0 = linkfilename1.substr(inpath.length() + 1, 9999);

//printl("linkfilename1:", linkfilename1);
//printl("linkfilename0:", linkfilename0);

	//inpath
	//neopath
	//waitsecs
	//sleepms
	//datasetcode

	while (requestloop()){};

	return "";

}

function requestloop()
{

	//on win95 this runs at same speed in full screen or windowed

	locks.unlock( "REQUEST*" ^ linkfilename1);
	locks.unlock( "REQUEST*" ^ replyfilename);
//print 'unlock locks,request*':linkfilename1
//print 'unlock locks,request*':replyfilename

	timex = var().time();
	breaktime = "X";

	//call getstationid(stationid)
	//if mv.lockrecord('messages',messages,stationid,'','') else null
	//indicate that we are active
//		messaging("CHECK");

	//TODO needs a break out of loop forever
	while (serviceloop()){}

	return false;

}

function serviceloop()
{

	//delete old response files every 10 requests or service loop
	if (nrequests.substr(-1, 1) == "0")
		gosub deleteoldfiles(inpath,".*\\.4$");

		//if tracing then
	std::wcout << mv.at(0);
	std::wcout << (var().time()).oconv("MTS");
	std::wcout << " " << mv.SYSTEM.a(17);
	std::wcout << " " << mv.ROLLOUTFILE.field2("\\", -1).field(".", 1, 1);
	std::wcout << " " << nrequests;
	std::wcout << " " << onalert.oconv("MD10P");
	std::wcout << " Listening ..."<< mv.at(-4)<<std::endl;

	//end else
	// print @(25,@crthigh/2-1):
	// print (date() 'd':' ':timex 'mts') 'c#30':
	// print @(25,@crthigh/2+1):
	// print ('requests: ':nrequests:' on alert: ':onalert 'md10p') 'c#30':
	// *print @(25,@crthigh/2+2):('on alert: ':onalert 'md10p') 'c#30':
	// end

	//prevent sleep in esc.to.exit
	timenow=var().ostime();
	mv.SYSTEM.r(25, timenow);
	mv.SYSTEM.r(26, timenow);

	if (mv.esctoexit())
		return false;

	//look for db stoppers in program directory
	for (int filen = 1; filen <= 2; filen++) {
		var filename = (globalend ^ VM ^ neopath ^ datasetcode ^ ".END").a(1, filen);
		if (filename.osfile()) {

			if (USER1.osread(filename)) {

				USER1.converter(var().chr(13) ^ var().chr(10), FM);

				if (USER1.a(1) == "") {
stopper:
						request1 = "STOPDB";
					return false;
				}

				var datetime = (var().date() ^ "." ^ (var().time()).oconv("R(0)#5")) + 0;

				var stopdatetime = (USER1.substr(1, 10)).iconv("D") ^ "." ^ (USER1.substr(11, 10)).iconv("MT");
				if (stopdatetime.isnum()) {
					if (datetime <= stopdatetime + 0)
						goto stopper;
				}

				filename.osdelete();

			}

		}

	};//filen;

	gosub flagserveractive();

//printl(inpath," ",".*\\.1$");

	var linkfilename;
	int nn=1000/sleepms*waitsecs;
	for (int ii=0;ii<=nn;ii++)
	{
		linkfilename=linkfilename.oslistf(inpath,".*\\.1$");
		if (linkfilename)
		{
			linkfilename=linkfilename.a(1);
			var(inpath^linkfilename).osrename(linkfilename1);
			break;
		}

//		if (var(neopath^"GLOBAL.END").osfile()!="") break;
//		if (var(neopath^datasetcode^".END").osfile()!="") break;

		linkfilename.ossleep(sleepms);

	}

	//if got file then process and quit serviceloop
	if (linkfilename1.osfile()!="") {

		linkfilenames = linkfilename0;

		return processlink();
	}


	//if time()>(timex+10*60) then

	//if not(tracing) and time()>(timestarted+10*60) then
	// request1='restart'
	// return 999999
	// end

	//handle midnight!
	if (var().time() < timestarted)
		timestarted = var().time();

	timex = var().time();

	//timeout if no activity
	timenow=var().ostime();
	var now = (var().date() ^ "." ^ (timenow.floor()).oconv("R(0)#5"));
	now+=0;
	//if timeouttime and now>timeouttime then return 999999

	gosub flagserveractive();

	//check for upgrade to Server

	//switch into interactive mode to check for operator input
	var s33 = mv.SYSTEM.a(33);
	mv.SYSTEM.r(33, "");

	//check for esc key to exit
	//if esc.to.exit() then return 999999

	var charx;
//		charx=keypressed();
	charx="";

	if (charx.length())
	{

		//charx.input(-1, 2);
		charx = charx.substr(1, 1).ucase();

		//esc
		if (charx == mv.INTCONST.a(1)) {
			mv.msg2("You have pressed the [Esc]  key to exit|press again to confirm|", "UB", buffer, "");
			while (true) {
				reply.input(-1);
			//BREAK;
			if (reply) break;;
			}//loop;
//				mv.msg2("", "DB", buffer, "");
			if (reply == mv.INTCONST.a(1))
				return false;
			if (reply == mv.INTCONST.a(7))
				charx = reply;
		}

		//r=restart
		if (charx == "R") {
			request1 = "RESTART";
			return false;
		}

		//f5
		if (charx == mv.PRIORITYINT.a(2))
			var("").execute();

		//f10
		if (charx == mv.INTCONST.a(7))
			var("RUNMENU " ^ mv.ENVIRONSET.a(37)).execute();

		if (charx != "") {

			//"d" = de bug
			//if charx='d' and @username='neosys' then
			// de bug
			// end

			gosub gettimeouttime();
		}

		//switch back to not interactive mode
		mv.SYSTEM.r(33, s33);

		gosub getbakpars();

		//perform backup if right time range and not backed up today
		if (bakreq && var().date() != lastbakattemptdate) {

			var dow = var().date() - 1 % 7 + 1;

			//override min/max times
			//if @username='neosys' and trim(@station)='sbcp1800' then

			if (charx == "B" || ( bakdows.index(dow, 1) && timex > minbaktime && timex < maxbaktime)) {

				if (testdata)
					//var("OFF").perform();
					return false;

				lastbakattemptdate = var().date();
				lastbakattemptdate.writev(mv.DEFINITIONS, "BACKUP", 1);

				USER4 = "";
				perform("FILEMAN BACKUP " ^ datasetcode ^ " " ^ bakdisk ^ " SYSTEM");

				//quit and indicate to calling program that a backup has been done
				//if tracing else
				mv.PSEUDO = "BACKUP";
				if (USER4)
					return false;
				// end

			}
		}
	}

	gosub deleteoldfiles(inpath, "^.*$");

	return true;

}

function processlink()
{

//printl("processlink:"^linkfilenames);

	nrequests += 1;
	mv.SYSTEM.r(35,0,0,nrequests);

	//get the earliest time possible for the log
	requeststarttime=var().ostime();
	mv.SYSTEM.r(25, requeststarttime);

	gosub gettimeouttime();

	var nlinkfiles = linkfilenames.count(FM) + 1;

	//find a request to process (only one since renaming to target file)
	for (int linkfilen = 1; linkfilen <= nlinkfiles; linkfilen++) {

		linkfilename1 = inpath ^ linkfilenames.a(linkfilen);

		//lock it to prevent other Servers from processing it
		//unlock locks,'request*':linkfilename1
		if (!(mv.lockrecord("", locks, "REQUEST*" ^ linkfilename1))) {
			if (tracing)
			{
				std::wcout<< "CANNOT LOCK LOCKS,";
				std::wcout<<("REQUEST*" ^ linkfilename1).quote();
				std::wcout<< std::endl;
			}
			continue;//goto nextlinkfile;
		}

		//open it or skip
		//var().osflush();
		if (!(linkfilename1.osopen())) {
			//TODO remove from future candidate files?
			mv.unlockrecord("", locks, "REQUEST*" ^ linkfilename1);
			if (tracing)
				std::wcout << "CANNOT OPEN " << linkfilename1.quote() << std::endl;
			var().ossleep(100);
			continue;//goto nextlinkfile;
		}

		//get the .1 file which contains the request
		//timex=time()+2
readlink1:
		//osbread request from linkfilename1 at 0 length 256*256-4
		var offset=0;
		USER0.osbread(linkfilename1, offset, 256 * 256 - 4);

		//if cannot read it then try again
		if (USER0 == "" && var().time() == timex) {
			//var().osflush();
			mv.unlockrecord("", locks, "REQUEST*" ^ linkfilename1);
			var().ossleep(100);
			linkfilename1.osclose();
			if (tracing)
				std::wcout << "CANNOT READ " << linkfilename1.quote() << std::endl;
			if (!(linkfilename1.osopen()))
				{}
			goto readlink1;
		}

//printl("processlink:read request ok");

		//hack to remove UTF16 BOM mark
		if (USER0.substr(1,1)=="\uFEFF")
			USER0.splicer(1,1,"");

		//cleanup the input request
		//TODO check ALL CHARACTERS are converted F8-FF and on all input/output
		//convert '&' to fm in request
		USER0.swapper(var().chr(13) ^ var().chr(10), FM);
		USER0.converter(var().chr(13) ^ var().chr(10), _FM_ _FM_ );
		USER0.swapper("\\\\", "\\");
		USER0.swapper("\\r", FM);
		USER0.swapper("%FF", RM);
		USER0.swapper("%FE", FM);
		USER0.swapper("%FD", VM);
		USER0.swapper("%FC", SVM);
		USER0.swapper("%FB", TM);
		USER0.swapper("%FA", STM);
		USER0.swapper("%F9", SSTM);
		USER0.trimmerb(FM);

USER0.outputl("USER0 after decode =");

		//extract and remove reply filename
		replyfilename = USER0.a(1);
		USER0.eraser(1, 0, 0);

		//check request is VERSION 3
		if (USER0.a(1) ne "VERSION 3") {
			if (tracing)
				std::wcout << "REQUEST TYPE MUST BE VERSION 3 BUT IS " << USER0.a(1).quote() << std::endl;
			mv.unlockrecord("", locks, "REQUEST*" ^ linkfilename1);
			continue;//goto nextlinkfile;
		}

		//lock the replyfilename to prevent other Servers from processing it
		//unlock locks,'request*':replyfilename
		if (!(mv.lockrecord("", locks, "REQUEST*" ^ replyfilename))) {
			if (tracing)
				std::wcout << "CANNOT LOCK LOCKS," << ("REQUEST*" ^ replyfilename).quote() << std::endl;
			mv.unlockrecord("", locks, "REQUEST*" ^ linkfilename1);
			continue;//goto nextlinkfile;
		}

//printl("lock locks,request*"^replyfilename);

		//delete then unlock the request file
		var ntries = 0;
deleterequest:
		linkfilename1.osclose();
		linkfilename1.osdelete();
		if (linkfilename1.osfile()) {
			//var().osflush();
			//garbagecollect;
			var().ossleep(100);
			ntries += 1;
			//if tracing then print 'could not delete ':linkfilename1
			if (ntries < 100)
				goto deleterequest;
			if (tracing)
				std::wcout << "COULD NOT DELETE "<< linkfilename1 << std::endl;
		}

		//leave these in place for the duration of the process
		//they should be cleared by unlock all somewhere at the end or beginning
		//unlock locks,'request*':replyfilename
		//unlock locks,'request*':linkfilename1

		//found a good one so process it
		return processrequest();

	};//linkfilen;

	return true;

}

function processrequest()
{

//printl("processrequest:",linkfilename1);

	nrequests += 1;

	//if tracing then
	//print @(0):@(-4):time() 'mts':' ':count(program.stack(),fm):
	//print @(0):@(-4):time() 'mts':' ':field2(linkfilename1,'\',-1):' ':field2(replyfilename,'\',-1):
	//print @(0):@(-4):time() 'mts':' ':field2(replyfilename,'\',-1):
	std::wcout << mv.at(0)<< mv.at(-4)<< (var().time()).oconv("MTS")<< " ";
	//end else
	// print @(25,@crthigh/2+1):
	// print ('processing request : ':nrequests) 'c#30':
	// end

	//clear out buffers just to be sure
	//request=''
	USER1 = "";
	//USER2=''
	USER3 = "Error: Response not set in Server.";
	USER4 = "";

	//only go on alert if we actually get the request
	//to avoid multiple processes going on alert for only one request
	onalert = onalertsecs;

	Serverfailure = 0;

	//request lines:

	//replyfilename (stripped out by the time we get here)

	//VERSION 3
	//6 (pre-fields after replyfilename)
	//127.0.0.1
	//127.0.0.1
	//https
	//session

	//database
	//usercode
	//password

	//request1 eg LOGIN
	//request2
	//etc.

	//analyse the input file
	var nconnectionfields = USER0.a(2);

	//extract and remove connection details
	//VERSION 3/ipno/hostname/https/session (nprefields is deleted)
	connection = USER0.field(FM, 1, nconnectionfields);
	connection.eraser(2);
	connection.converter(FM, VM);
	USER0 = USER0.field(FM,  nconnectionfields+1, 99999);

	//save connection details
	mv.SYSTEM.r(40, connection);
//connection.outputl("connection=");
//USER0.outputl("USER0 after removal of connection=");

	//get and remove the dataset user and password
	dataset = USER0.a(1).ucase();
	username = USER0.a(2).ucase();
	password = USER0.a(3).ucase();
	USER0 = USER0.field(FM, 4, 99999);

	//remove CACHE prerequest
	if (USER0.a(1) == "CACHE")
		USER0.eraser(1);

	//USER0 is now the request
	//get the first 5 parts
	request1 = USER0.a(1).ucase();
	request2 = USER0.a(2);
	request3 = USER0.a(3);
	request4 = USER0.a(4);
	request5 = USER0.a(5);
//USER0.outputl("USER0 finally=");

	if (logfilename!="") {

		var datex = var().date();
		timex = requeststarttime;

		var tt = var().chr(13) ^ var().chr(10) ^ "<Message ";
		tt ^= " Date=" ^ mv.xmlquote(datex.oconv("D"));
		tt ^= " Time=" ^ mv.xmlquote(timex.oconv("MTS"));
		tt ^= " DateTime=" ^ mv.xmlquote(datex.oconv("DJ-") ^ "T" ^ timex.oconv("MTS") ^ "." ^ timex.field(".", 2, 1));
		tt ^= " User=" ^ mv.xmlquote(username);
		tt ^= " File=" ^ mv.xmlquote(replyfilename.field2("\\", -1));
		//remote_addr remote_host https
		tt ^= " IP_NO=" ^ mv.xmlquote(connection.a(1, 2));
		tt ^= " Host=" ^ mv.xmlquote(connection.a(1, 3));
		tt ^= " HTTPS=" ^ mv.xmlquote(connection.a(1, 4));
		tt ^= ">" ^ (var().chr(13) ^ var().chr(10));

		tt ^= "<Request ";

		logx = USER0;
		gosub convlogx();
		logx.converter("^", FM);
		logx.r(1, request1);
		if (logx.a(1)!="")
			tt ^= " Req1=" ^ logx.a(1).quote();
		if (logx.a(5)!="")
			tt ^= " Req2=" ^ logx.a(5).quote();
		if (logx.a(6)!="")
			tt ^= " Req3=" ^ logx.a(6).quote();
		if (logx.a(7)!="")
			tt ^= " Req4=" ^ logx.a(7).quote();
		if (logx.a(8)!="")
			tt ^= " Req5=" ^ logx.a(8).quote();
		var req6up = logx.field(FM, 9, 9999);
		req6up.converter(FM, "^");
		if (req6up.length())
			tt ^= " Req6up=" ^ req6up.quote();

		tt ^= "/>" ^ (var().chr(13) ^ var().chr(10));
		tt.transfer(logx);
		gosub writelogx2();

		mv.osbwritex("<DataIn>", logfilename, logfilename, logptr);

	}

	var anydata = 0;

	if (request2 == "JOURNALS") {
		request2 = "BATCHES";
		if (request3.count("*") == 3 && request3.substr(-1, 1) == "*")
			request3.splicer(-1, 1, "");
	}

	//print the request (hide dataset and password)
	if (tracing) {
		var tt = username ^ " " ^ USER0;
		tt.convert(FM, " ");

		var t2 = connection.a(1, 2);
		if (connection.a(1, 3) != t2)
			t2 ^= " " ^ connection.a(1, 3);

		t2.trimmer();
		std::wcout << " " << (t2 ^ " " ^ tt).trim().quote() << std::endl;
	}

	linkfilename2 = replyfilename.splice(-1, 1, 2);
	
	//should be made into a MvLib
	linkfilename2.swapper("/\\",SLASH^SLASH);
	//if (linkfilename2.index(":"))
	//{
	//	linkfilename2.swapper(":",SLASH);
	//	linkfilename2="/cygdrive/"^linkfilename2;
	//}

	linkfilename3 = replyfilename.splice(-1, 1, 3);
	
	//should be made into a MvLib
	linkfilename3.swapper("/\\",SLASH^SLASH);
	//if (linkfilename3.index(":"))
	//{
	//	linkfilename3.swapper(":",SLASH);
	//	linkfilename3="/cygdrive/"^linkfilename3;
	//}

	//save the response file name
	//so that if Server fails then net the calling program can still respond
	mv.PRIORITYINT.r(100, linkfilename3);

	var linkfilename2size = linkfilename2.osfile().a(1);
	if (linkfilename2size > maxstringsize) {

		if (linkfilename2.osopen()) {

			//read blocks of iodat
			for (int blockn = 1; blockn <= nblocks; blockn++)
				datx[blockn]="";
//				datx.initarray("");
			for (int blockn = 1; blockn <= nblocks; blockn++) {

				//osbread datx(blockn) from linkfilename2 at ((blockn-1)*inblocksize) length inblocksize
				var offset=(blockn - 1) * inblocksize;
				mv.osbreadx(datx[blockn], linkfilename2, linkfilename2, offset, inblocksize);

				//BREAK;
				if (!((datx[blockn]).length())) break;;

				//hack to remove UTF16 BOM mark
				if (blockn==1&&datx[blockn].substr(1,1)=="\uFEFF")
					datx[blockn].splicer(1,1,"");

				//avoid hexcode spanning block end by moving one or two bytes backwards
				if (blockn > 1) {
					var tt = ((datx[int(blockn - 1)]).substr(-2, 2)).index("%", 1);
					if (tt) {
						datx[int(blockn - 1)] ^= (datx[blockn]).substr(1, tt);
						(datx[blockn]).splicer(1, tt, "");
					}
				}

			};//blockn;

			//unescape all blocks
			var lendata = 0;
			for (int blockn = 1; blockn <= nblocks; blockn++) {

				if ((datx[blockn]).length()) {

					//hack to remove UTF16 BOM mark
					if (blockn==1&&datx[blockn].substr(1,1)=="\uFEFF")
						datx[blockn].splicer(1,1,"");

					//output to log
					if (logfilename) {

						//start after the last <datain>
						if (!anydata) {
							anydata = 1;
							logptr += 8;
						}

						logx = datx[blockn];
						gosub writelogx();

					}

					for (int ii = 0; ii <= 36; ii++)
						datx[blockn].swapper(hexx[ii], var().chr(ii));
					for (int ii = 38; ii <= 255; ii++)
						datx[blockn].swapper(hexx[ii], var().chr(ii));
					datx[blockn].swapper(hexx[37], var().chr(37));

					lendata += (datx[blockn]).length();

				}

			};//blockn;

			//check max iodat size <= maxstringsize
			if (lendata > maxstringsize) {
				USER1 = "";
				USER3 = "Error: Maximum record size of 64Kb exceeded in Server";
				Serverfailure = 1;

				//otherwise join the blocks
			}else{
				USER1 = "";
				for (int blockn = 1; blockn <= nblocks; blockn++) {
					USER1 ^= datx[blockn];
					datx[blockn] = "";
				};//blockn;
			}

			//cannot open linkfilename2 means no iodat
			}else{
cannotopenlinkfilename2:
				Serverfailure = 1;
				USER1 = "";
			USER3 = "Error: Server cannot open " ^ linkfilename2;
		}

	}else{

		if (!linkfilename2size) {

			USER1 = "";

		}else{

			if (!(linkfilename2.osopen()))
				goto cannotopenlinkfilename2;

			//osread iodat from linkfilename2 else goto cannotopenlinkfilename2
			var offset=0;
			mv.osbreadx(USER1, linkfilename2, linkfilename2, offset, maxstringsize);

			//hack to remove UTF16 BOM mark
			if (USER1.substr(1,1)=="\uFEFF")
				USER1.splicer(1,1,"");

			//unescape
			//for ii=0 to 255

			//output to log
			if (logfilename) {

				//start after the last <datain>
				if (!anydata) {
					anydata = 1;
					logptr += 8;
				}

				logx = USER1;
				gosub writelogx();
			}

			for (int ii = 0; ii <= 36; ii++)
				USER1.swapper(hexx[ii], var().chr(ii));
			for (int ii = 38; ii <= 255; ii++)
				USER1.swapper(hexx[ii], var().chr(ii));
			//unescape %25 to % LAST!
			USER1.swapper(hexx[37], var().chr(37));

			// next i

		}

	}

	if (linkfilename2.osopen()) {
		linkfilename2.osclose();
		linkfilename2.osdelete();
		//osread _USER3 from linkfilename3 else USER3=''
	}

	if (logfilename!="") {

		if (anydata) {
			logx = "</DataIn>";
			gosub writelogx2();
		}

		//to be overwritten unless fails
		logx = "</Message></Log>";
		gosub writelogx3();

	}

	//update security table every few secs and every login
	if (request1 == "LOGIN" || var("036").index(var().time()).substr(-1, 1))
		gosub getsecurity();

	//validate username and password (like login)
	//and assume that identity if ok
	gosub validate();

	mv.SYSTEM.r(2, linkfilename2);

	//get the current program stack
//	var stack = programstack(nostack);

	try
	{
		gosub process();
	}
//dont catch general MVExceptions to force debugging
//	catch (MVException& mvexception)
	catch (std::wstring& message)
	{
		USER3 = message;
		//gosub properunlock();
		gosub formatresponse();
	}
	catch (var& message)
	{
		USER3 = message;
		//gosub properunlock();
		gosub formatresponse();
	}

	//restore the program stack
	//limit on 299 "programs" and dictionary entries count as 1!!!
//	xx = programstack(stack);

	gosub requestexit();

	if (halt)
		return false;

	if (request1 == "STOPDB" && USER3 == "OK")
		return false;

	if (request1 == "RESTART" && USER3 == "OK")
		return false;

	return true;

}

subroutine login()
{

	USER1 = "";
	USER4 = "";

	//custom login routine
	//returns iodat (cookie string "x=1&y=2" etc and optional comment)
	//if (xx.read(md, "LOGIN.NET")) {
		USER1 = "";
		USER4 = "";
		mv.loginnet(dataset, username, USER1, USER4);
		if (USER1 == "") {
			USER3 = USER4;
			return;
		}
	//}

	if (!(USER1))
		USER1 = "X=X";
	USER3 = var("OK " ^ USER4).trim();

	//record the last login per user
	var users;
	if (users.open("USERS")) {
		var userrec;
		if (userrec.read(users, username)) {
			//save prior login
			userrec.r(15, userrec.a(13));
			userrec.r(16, userrec.a(14));
			//save current login
			var datetime = var().date() ^ "." ^ (var().time()).oconv("R(0)#5");
			userrec.r(13, datetime);
			userrec.r(14, mv.SYSTEM.a(40, 2));
			userrec.write(users, username);
		}
	}

	return;
}

subroutine validate()
{
	invaliduser = "Error: Invalid username and/or password";
//printl("validate");
	//encrypt the passwors and check it
	var encrypt0 = mv.encrypt2(password ^ "");
	var usern;
	var systemrec;
	var tt;
	var connections;

	if (username=="NEOSYS") goto userok;
	else if (mv.SECURITY.locate(username, usern, 1)) {
		if (encrypt0 == (mv.SECURITY.a(4, usern, 2)).field(TM, 7, 1)) {

			//determine allowed connections
			connections = mv.SECURITY.a(6, usern);

			//skip old integer sleep times
			if (connections.match("1N0N"))
				connections = "";

			if (!connections) {

				//look for ip numbers in following users of the same group
				int nn = (mv.SECURITY.a(6)).count(VM) + 1;
				for (int ii = usern + 1; ii <= nn; ii++) {
					connections = mv.SECURITY.a(6, ii);

					//skip old integer sleep times
					if (connections.match("1N0N"))
						connections = "";

				//BREAK;
				if (!(!connections && mv.SECURITY.a(1, ii + 1) != "---")) break;;
				};//ii;

				//otherwise use system default
				if (!connections)
					connections = mv.SYSTEM.a(39);

				//otherwise use traditional private ip ranges
				//cannot implement this until check existing clients installations
				//if connections else connections='192.168.*.*;10.*.*.*'

			}

			//convert wildcards
			if (connections.substr(-1, 1) != " ") {

				connections.converter(",; ", SVM ^ SVM ^ SVM);
				int nn = connections.dcount(SVM);
				for (int ii = 1; ii <= nn; ii++) {

					var connectionx = connections.a(1, 1, ii).quote();
					connectionx.swapper("*", "\"0N\"");

					//invert the bits that are "quoted" to suit the match syntax '"999"0n'
					//""0n"."0n"" will become 0n"."0n
					connectionx.swapper("\"\"", "");

					//pad missing dots so that 192.* becomes 192"."0n"."0n"."0n
					var ndots = connectionx.count(".");
					if (ndots < 3)
						connectionx ^= var("\".\"0N").str(3 - ndots);

					connections.r(1, 1, ii, connectionx);
				};//ii;

				//buffer it (space prevents reassement even if none)
				//trailing space also means wildcards have been converted
				mv.SECURITY.r(6, usern, 0, connections ^ " ");
			}

			//check if allowed to connect from xxx

			if (connections.trim()) {
				int nn = connections.dcount(SVM);
				int ii;
				for (ii = 1; ii <= nn; ii++) {
					var connectionx = (connections.a(1, 1, ii)).trimb();
				//BREAK;
				if ((connection.a(1, 2)).match(connectionx)) break;;
				};//ii;
				if (ii > nn) {
					invaliduser = username ^ " is not authorised to login" ^ FM ^ "on this computer (IP Number: " ^ connection.a(1, 2) ^ ")";
						return;
					}
				}

userok:
				USERNAME=username;
				tt = connection.a(1, 2);
				if (connection.a(1, 3) != tt)
					tt ^= "_" ^ connection.a(1, 3);
			tt.converter(". ", "_");
			mv.STATION=tt;
			invaliduser = "";
		}
	}else{

		//check revelation system file
		if (systemrec.read(mv.DEFINITIONS, "SYSUSER*"^username)) {
			//if systemrec<7>=encrypt0 and (speed<1.5 or mode='sleep') then ok=1
			if (systemrec.a(7) == encrypt0)
				goto userok;
		}
username.outputl("cant find user in definitions, security = ");

	}

	//return

}

subroutine requestexit()
{
	var().unlockall();

	if (USER3 == "")
		USER3 = "Error: No response";

printl("requestexit:"^USER3.quote());

	if (USER3.index("ERROR NO:", 1))
		mv.logger("Server", USER3);

	USERNAME=Serverusername;
	mv.STATION=Serverstation;

	//if USER3[1,6].ucase()='ERROR:' then iodat=''

	//have to call it here as well :(
	requeststoptime=var().ostime();

	var rawresponse = USER3;
	rawresponse.converter(var().chr(13) ^ var().chr(10), "|");

	if (logfilename!="") {

		var secs = (requeststoptime - requeststarttime).oconv("MD20P");
		logx = "<Response";
		logx ^= " ProcessingSecs=" ^ secs.quote();
		logx ^= ">";
		gosub writelogx2();

		//convert non ascii to hexcode
		logx = USER3;
		//escape % to %25 FIRST!
		logx.swapper("%", "%25");
		//for (int ii = 128; ii <= 255; ii++)
		for (int ii = 249; ii <= 255; ii++)
			logx.swapper(var().chr(ii), hexx[ii]);
		for (int ii = 0; ii <= 31; ii++)
			logx.swapper(var().chr(ii), hexx[ii]);
		gosub writelogx();

		logx = "</Response>" ^ (var().chr(13) ^ var().chr(10));
		iodatlen = USER1.length();
		if (iodatlen)
			logx ^= "<DataOut>";
		gosub writelogx2();

	}

	if (USER1 == "%DIRECTOUTPUT%") {

		logx = USER1;
		logx.swapper("%", "%25");
		gosub writelogx();

	}else{

		var("").oswrite(linkfilename2);
		if (linkfilename2.osopen()) {

			//split into blocks and convert to escape chars
			for (int blockn = 1; blockn <= nblocks; blockn++)
				datx[blockn]="";
//			datx.initarray("");
			var ptr = 0;
			for (int blockn = 1; blockn <= nblocks; blockn++) {
				var blk = USER1.substr(1, outblocksize);
				USER1.splicer(1, outblocksize, "");
			//BREAK;
			if (!blk.length()) break;;

				//escape the escape character first!
				blk.swapper("%", "%25");

				//allow top eight pick language characters to pass through
				//to whatever corresponding unicode characters are desired by front end
				for (int ii = 249; ii <= 255; ii++) {
					blk.swapper(var().chr(ii), hexx[int(ii)]);
					//should not be done per block but is code economic
					USER3.swapper(var().chr(ii), hexx[int(ii)]);
				};//ii;

				//write BEFORE converting control characters
				//since writing restores 10-17 back up to F8-FF
				mv.osbwritex(blk, linkfilename2, linkfilename2, ptr);
				ptr += blk.length();

				//encode control characters to show in log
				//should convert x10-x17 back to proper characters
				for (int ii = 0; ii <= 31; ii++) {
					blk.swapper(var().chr(ii), hexx[int(ii)]);
					//should not be done per block but is code economic
					USER3.swapper(var().chr(ii), hexx[int(ii)]);
				};//ii;

				if (logfilename!="") {
					blk.transfer(logx);
					gosub writelogx();
				}

				blk = "";

			};//blockn;

			linkfilename2.osclose();

		}else{

			USER3 = "ERROR: Server cannot create temp " ^ linkfilename2;

		}

	}

	//try to flush file open
	if (linkfilename2.osopen())
		linkfilename2.osclose();

	if (logfilename!="") {
		var tt = "";
		if (iodatlen)
			tt ^= "</DataOut>";
		tt ^= "</Message>" ^ (var().chr(13) ^ var().chr(10));
		mv.osbwritex(tt, logfilename, logfilename, logptr);
		logptr += tt.length();

		mv.osbwritex("</Log>", logfilename, logfilename, logptr);

	}

	//swap '|' with wchar_t(13) in USER3
	//swap fm with wchar_t(13) in USER3

//printl("writing response:", USER3, linkfilename3);
	//write the response
	mv.oswritex(USER3, linkfilename3);

	//trace responded
	requeststoptime=var().ostime();
	if (tracing) {

		std::wcout << "Responding in "<< (requeststoptime - requeststarttime).oconv("MD20P")<< " SECS "<< rawresponse << std::endl;
		//print linkfilename1
	}

	//flush
	//suspend 'dir>nul'

	//if tracing then print

	return;

}

subroutine exit()
{

	if (logfilename!="")
		logfilename.osclose();

	//remove lock indicating processing
	//gosub sysunlock

	var().unlockall();

	//restorescreen(origscrn, origattr);

	//remove flag that this dataset is being served ("listened")
	//osdelete inpath:serverflagfilename

	//get into interactive mode
	//system<33>=origbatchmode
	mv.SYSTEM.r(33, "");
	mv.PRIVILEGE=origprivilege;

	if (request1 == "RESTART") {
		//chain 'Server'
		mv.SYSTEM.r(35, nrequests);
		//msg='restart'
		//stop

		origsentence.swapper(" INTRANET", "");
		origsentence.chain();
	}

	//BREAK OFF;
	//BREAK ON;

	//if (origbatchmode || request1 == "STOPDB" || halt) {
	//	//BREAK OFF;
		//var("OFF").perform();
	//}

	//msg is @user4
	USER4 = "TERMINATED OK";

	var().stop();

}

subroutine process()
{
printl("process:"^USER0);
	//process the input
	//////////////////

	//failure in Server above
	if (Serverfailure) {

	//invalid username or password or connection
	}else if (invaliduser!="") {
		USER1 = "";
		USER3 = invaliduser;
//call oswrite(username:fm:password:fm:encrypt0:fm:mv.SECURITY,date()[-3,3]:time():'.$$$')
		//if no request then possibly some failure in file sharing

	}else if (USER0 == "") {
		USER1 = "";
		USER3 = "Error: No request";

	//empty loopback to test connection (no username or password required)
	}else if (request1 == "TEST") {
		//iodat='' if they pass iodat, pass it back
		USER3 = "OK";

	//check if can login
	}else if (request1 == "LOGIN") {

		gosub login();

	//find index values
	}else if (request1.substr(1, 14) == "GETINDEXVALUES") {

		getindexvalues();

	//select some data
	}else if (request1 == "SELECT") {

		select();

	//lock a record
	}else if (request1 == "LOCK" || request1 == "RELOCK") {

		filename = request2;
		keyx = request3;
		sessionid = request4;

		//lockduration is the number of minutes to automatic lock expiry
		lockdurationinmins = request5;

		gosub lock();
		sessionid.transfer(USER1);

	}else if (request1 == "UNLOCK") {

		filename = request2;
		keyx = request3;
		sessionid = request4;

		gosub unlock();

		//read a record

	}else if (request1 == "READ" || request1 == "READO" || request1 == "READU") {

		withlock = request1 == "READU";
		updatenotallowed = "";
		USER1 = "";
		sessionid = "";

		filename = request2;
		keyx = request3;
		lockdurationinmins = request4;
		var readenvironment = request5;
		win.templatex = readenvironment;

		//reduce chance of using old common
		for (int ii=0;ii<10;ii++)
			win.registerx[ii]="";

		var library="";
/*TODO reimplement as simple external function
		if (!library.load(filename))
			{};//throw MVException("MVCipc() library.load() " ^ filename.quote() ^ " unknown filename");
*/
		newfilename="";
		//if (library) newfilename=library.call("GETALIAS");

		//disallow read/write to unknown files
		if (false&&newfilename == "") {
			USER3 = request1 ^ " " ^ filename.quote() ^ " is not allowed";
			return;
		}
		if (newfilename == "") newfilename = filename;
		
		filename2 = mv.singular(newfilename);
		filename2.converter(".", " ");
		//if security(filename2:' access',msg0,'') else
		// if security('#':filename2:' access ':quote(keyx),msg2,'') else
		// transfer msg0 to USER3
		// gosub formatresponse
		// return
		// end
		// end
		if (!filesecurity("ACCESS")) return;

		//security check - cannot lock so cannot write or delete
		if (withlock) {
			//if 1 then
			if (mv.authorised(filename2 ^ " UPDATE", updatenotallowed, ""))
				{}
			if (mv.authorised(filename2 ^ " CREATE", createnotallowed, ""))
				{}
			//if updatenotallowed and createnotallowed then
			// USER3='error: sorry, you are not authorised to create or update records in the ':lcase(filename):' file.'
			// *USER3='error: lock not authorised'
			// gosub formatresponse
			// return
			// end
		}

		var file;
		if (!file.open(filename)) {
			gosub badfile();
			return;
		}

		if (library) {
			keyx.transfer(ID);
/*TODO reimplement as simple external function
			library.call("PREREAD");
*/
			mv.DATA = "";
			ID.transfer(keyx);
		}

		var autokey = 0;
		if (keyx == "" || keyx.substr(1, 1) == "*" || keyx.substr(-1, 1) == "*" || keyx.index("**", 1)) {

			//must provide a key unless locking
			if (!withlock) {
				USER3 = "Error: NEOSYS.NET Key missing and not READU";
					gosub formatresponse();
					return;
				}

getnextkey:

				//setup environment for def.sk
			win.wlocked = 0;
			RECORD = "";
			ID = keyx;
			win.datafile = filename;
			win.srcfile = file;

//TODO			generalsubs("DEF.SK." ^ readenvironment);
//			keyx = isdflt;
keyx="";

			if (keyx == "") {
				USER3 = "Error: Next number was not produced" ^ FM ^ USER4;
				gosub formatresponse();
				return;
			}

			autokey = 1;

		}

		sessionid = "";
		if (withlock) {
			//lockduration is the number of minutes to automatic lock expiry
			//lockdurationinmins=request4
			lockdurationinmins = 10;
			USER3 = "";

			gosub lock();

			//if cannot lock then get next key
			if (USER3 == "NOT OK" && autokey)
				goto getnextkey;

			//cannot do this because should return the record but without a lock
			//even if a lock requested
			//if USER3 then
			// gosub formatresponse
			// return
			// end
		}

		mv.FILEERRORMODE = 1;
		mv.FILEERROR = "";
		if (USER1.read(file, keyx)) {

			//if record already on file somehow then get next key
			if (autokey)
				goto getnextkey;

			//prevent update
			if (withlock && updatenotallowed) {
				if (sessionid)
					gosub unlock();
				// *do this after unlock which sets response to ok
				//USER3=updatenotallowed
				//gosub formatresponse
				//return
				lockauthorised = 0;
				sessionid = "";
			}

			if (withlock && sessionid == "") {
				USER3 = "Error: CANNOT LOCK RECORD";
			}else{
				USER3 = "OK";
			}
			if (sessionid)
				USER3 ^= " SESSIONID " ^ sessionid;
		}else{
			//if @file.error<1>='100' then
			//no file error for jbase
			if (!mv.FILEERROR || mv.FILEERROR.a(1) == "100") {

				//prevent create
				if (withlock) {
					if (createnotallowed) {
						if (sessionid)
							gosub unlock();
						//do this after unlock which sets response to ok
						USER3 = createnotallowed;
						gosub formatresponse();
						return;
					}
				}

				//USER3='error: ':quote(keyx):' does not exist in the ':quote(newfilename):' file'
				USER1 = "";
				USER3 = "Error: NO RECORD";
				if (sessionid)
					USER3 ^= " SESSIONID " ^ sessionid;

				//response/@user3/recordkey may be used in postread
				if (autokey) {
					//USER3:=' recordkey ':keyx
					var tt = keyx;
					//horrible cludge to allow space in recordkey to be understood in client.htm
					tt.swapper(" ", "{20}");
					USER3 ^= " RECORDKEY " ^ tt;
				}

			}else{
				gosub geterrorresponse();
			}
		}

		if (withlock) {
			if (!lockauthorised)
				USER3 ^= " LOCK NOT AUTHORISED";
		}

		//postread (something similar also in select2)
		if (library) {

			//simulate environment for postread
			win.srcfile = file;
			win.datafile = filename;
			if (!DICT.open("dict_"^win.datafile)) {
				USER3 = "Server::gosub process() DICT." ^ win.datafile ^ " file cannot be opened";
				gosub formatresponse();
				if (sessionid)
					gosub unlock();
				return;
			}
			keyx.transfer(ID);
			USER1.transfer(RECORD);
			win.orec = RECORD;
			win.wlocked = sessionid;
			var origresponse = USER3;
			//_USER3=''
			USER4 = "";
			var resetx = 0;
/*TODO reimplement as simple external function
			library.call("POSTREAD");
*/
			mv.DATA = "";

//restore this programs environment
			RECORD.transfer(USER1);
			ID.transfer(keyx);

			USER1.cropper();

			//postread can request abort by setting msg or resetx>=5
			if (resetx >= 5 || USER4) {
				if (withlock) {
					gosub unlock();
					//win.wlocked=0
				}
				//if msg then msg='error: ':msg
				USER3 = USER4;
				if (USER3 == "")
					USER3 = "ACCESS REFUSED";
				USER1 = "";
				gosub formatresponse();
				//USER3='error: ':USER3
			}else{
				USER3 = origresponse;
				//postread may have provided a record where non-was found
				//only if it unlocks it as well! otherwise
				//removal of no record will cause failure in client
				if (!win.wlocked && USER1) {
					USER3.swapper("Error: NO RECORD", "");
					USER3.trimmerb();
					if (USER3 == "")
						USER3 = "OK";
				}

			}

			//postread may have unlocked the record
			//(and removed it from the locks file)
			//but make sure
			if (sessionid && !win.wlocked) {

				var storeresponse = USER3;
				gosub unlock();
				USER3 = storeresponse;

				//remove session id
				var tt = USER3.index("SESSIONID", 1);
				if (tt) {
					USER3.splicer(tt, var("SESSIONID " ^ sessionid).length(), "");
					USER3.trimmerb();
					if (USER3 == "")
						USER3 = "OK";
				}

			}

			//do not indicate record found
			//otherwise delete button will be enabled and deleting a record
			//that does not exist is disallowed but lock will be unlocked
			//if iodat then
			// USER3='ok'
			// if sessionid then USER3:=' sessionid ':sessionid
			// end

		}

		//prevent reading passwords postread and postwrite
		if (filename == "DEFINITIONS" && keyx == "SECURITY")
			USER1.r(4, "");

		USER1.cropper();

	}else if (request1 == "WRITEU" || request1 == "DELETE" || request1 == "WRITE") {

		filename = request2;
		keyx = request3;
		var fieldno = request4;
		sessionid = request5;

//		routineexists=routines.count(filename.towstring());
//		MVRoutine *routine;
//		if (routineexists)
//		{
//			routine=routines[filename.towstring()];
//			newfilename=routine->execute("GETALIAS");
//		}
//		else
		var library="";
/*TODO reimplement as simple external function
		if (!library.load(filename))
			{};
*/
	   newfilename="";
		//if (library) newfilename=library.call("GETALIAS");

		//disallow read/write to unknown files
		if (false&&newfilename == "") {
			USER3 = request1 ^ " " ^ filename.quote() ^ " is not allowed";
			return;
		}
		if (newfilename == "") newfilename = filename;

		filename2 = mv.singular(newfilename);
		filename2.converter(".", " ");

		//double check allowed access to file
		if (!filesecurity("ACCESS")) return;

		//if security(filename2:' access',msg0,'') else
		// if security('#':filename2:' access ':quote(keyx),msg2,'') else
		// transfer msg0 to USER3
		// gosub formatresponse
		// return
		// end
		// end

		//simulate environment

		//reduce chance of using old common
		for (int ii=0;ii<10;ii++)
			win.registerx[ii]="";

		ID = keyx;
		//@record=iodat
		USER1.transfer(RECORD);
		win.datafile = filename;
		//not really needed because pre/post code should assume that it is win.wlocked
		//but some code does not know that (eg postread called from postwrite)
		win.wlocked = sessionid;
		win.saverec = !(request1 == "DELETE");
		win.deleterec = (request1 == "DELETE");

		//trim excess field and value marks
		RECORD.cropper();

		if (!win.srcfile.open(win.datafile)) {
			USER3 = win.datafile.quote() ^ " file is not available";
			return;
		}

		if (!DICT.open("dict_"^win.datafile)) {
			USER3 = ("DICT." ^ win.datafile).quote() ^ " file is not available";
			return;
		}

		//make sure that the record is already locked to the user
		lockkey = filename ^ "*" ^ ID;
		if (!lockx.read(locks, lockkey))
			lockx = FM ^ FM ^ FM ^ FM ^ "NO LOCK RECORD";
		if (sessionid != lockx.a(5)) {
			USER3 = "Somebody has updated this record." _VM_ "Your update cannot be applied." "" _VM_ "The session id does not agree " ^ lockx.a(5).quote();
			gosub formatresponse();
			return;
		}

		//get a proper lock on the file
		//possibly not necessary as the locks file entry will prevent other programs
		//proper lock will prevent index mfs hanging on write

		win.valid = 1;
		gosub properlock();
		if (!win.valid)
			return;

		if (!win.orec.read(win.srcfile, ID))
			win.orec = "";

		//trim excess field and value marks
		win.orec.cropper();

		//double check not updated by somebody else
		//nb this does not work for delete until client provides
		//record or at least date_time in delete
		//not implemented for timesheets due to possibility of old mac client not working
		if (request1 != "DELETE" && filename != "TIMESHEETS") {
			// *cannot do this unless
			//a) return new record (done) or at least date_time in writeu to client
			//b) provide record in delete
			if (allcols != "") {
				var dictrec;
				//if (dictrec.reado(allcols, filename ^ "*DATE_TIME")) {
				if (dictrec.read(allcols, filename ^ "*DATE_TIME")) {
					var datetimefn = dictrec.a(2);
					var olddatetime = win.orec.a(datetimefn);
					var newdatetime = RECORD.a(datetimefn);
					if (olddatetime && olddatetime != newdatetime) {
						USER3 = "Somebody else has updated this record." _VM_ "Your update cannot be applied." "" _VM_ "The time stamp does not agree";
						gosub formatresponse();
						gosub properunlock();
						return;
					}
				}
			}
		}

		//detect if custom postwrite or postdelete called
		var postroutine = 0;

		if (request1 == "WRITEU" || request1 == "WRITE") {

			//prevent writing an empty record
			if (RECORD == "") {
emptyrecorderror:
					USER3 = "Write empty data record is disallowed.";
				gosub properunlock();
				gosub formatresponse();
				return;
			}

			//double check that the record has not been updated since read
			//cannot do this unless during write (not writeu) we pass back the new timedate
			//readv datetimefn from @dict,'date_time',2 then
			// if @record<datetimefn> ne win.orec<datetimefn> then
			// _USER3='somebody else has updated this record." _VM_ "your update cannot be applied'
			// gosub properunlock
			// gosub formatresponse
			// return
			// end
			// end

			//custom prewrite processing
			if (library) {
				//call @updatesubs('prewrite')
/*TODO reimplement as simple external function
				library.call("PREWRITE");
*/
				mv.DATA = "";
			}

			if (!win.valid) {
				gosub properunlock();
				USER3 = USER4;
				gosub formatresponse();
				return;
			}

			//allow for prewrite to change the key (must not lock the new key)
			//prewrite must not unlock the record because write will fail by index mfs hanging if any
			//is the above true? because unlock just below will
			//remove the lock file record
			//and actually the unlock will fail if it cannot true lock the record
			//daybook.subs4 unlocks and it works fine and will leave lock hanging if it does not
			if (ID != keyx) {

				gosub unlock();
				gosub properunlock();

				keyx = ID;

				//cannot place a lock file entry if the prewrite has locked the new key
				file.unlock( keyx);

				gosub lock();
				if (USER3 != "OK")
					return;

				gosub properlock();
				if (!win.valid)
					return;

			}

			RECORD.cropper();

			//check for empty record again in case updatesubs screwed up
			if (RECORD == "")
				goto emptyrecorderror;

			//failsafe in case prewrite unlocks key?
			//gosub properlock

			RECORD.write(win.srcfile, keyx);

			//custom post write processing
			if (library) {
				postroutine = 1;
/*TODO reimplement as simple external function
				library.call("POSTWRITE");
*/
				mv.DATA = "";
			}

			//send back revised data or nothing
			//nb data is now '' to save space so always send back data unless @record is cleared
			if (RECORD == USER1) {
				USER1 = "";
			}else{
				USER1 = RECORD;
			}

			//prevent reading passwords postread and postwrite
			if (filename == "DEFINITIONS" && keyx == "SECURITY")
				USER1.r(4, "");

		}else if (request1 == "DELETE") {

			//ensure that deletion works on the orig record
			RECORD = win.orec;

			//prevent deleting if record does not exist
			if (RECORD == "") {
				USER3 = "Cannot delete because " ^ keyx.quote() ^ " does not exist in the " ^ newfilename.quote() ^ " file";
				gosub formatresponse();
				return;
			}

			//check allowed to delete
			//if security(filename2:' delete',msg,'') else
			// USER3=msg
			// gosub formatresponse
			// return
			// end
			if (!filesecurity("DELETE")) return;

			//custom predelete processing
			if (library) {
				//call @updatesubs('predelete')
/*TODO reimplement as simple external function
				library.call("PREDELETE");
*/
				mv.DATA = "";
			}
			if (!win.valid) {
				gosub properunlock();
				USER3 = USER4;
				gosub formatresponse();
				return;
			}

			win.srcfile.deleterecord(keyx);

			//custom post delete processing
			if (library) {
				postroutine = 1;
/*TODO reimplement as simple external function
				library.call("POSTDELETE");
*/
				mv.DATA = "";
			}

			//send back no iodat
			USER1 = "";

			}

			//remove locks file entry
		if (request1 != "WRITE") {

			locks.deleterecord(lockkey);
			relistlocks = 1;

			//unlock local lock
			win.srcfile.unlock( keyx);

		}else{
			gosub properunlock();
		}

		//even postwrite/postdelete can now set invalid (to indicate invalid mode etc)
		if (win.valid) {
			USER3 = "OK";
		}else{
			USER3 = "Error:";
		}

		if (request1 != "DELETE") {
			var tt = ID;
			//USER3:=' recordkey ':@id
			//horrible cludge to allow space in recordkey to be understood in client.htm
			tt.swapper(" ", "{20}");
			USER3 ^= " RECORDKEY " ^ tt;
			if (sessionid)
				USER3 ^= " SESSIONID " ^ sessionid;
		}

		//pre and post routines can return warnings/notes in msg

		if (USER4)
			USER3 ^= " " ^ USER4;

		gosub formatresponse();

		//if postroutine else call flush.index(filename)
		mv.flushindex(filename);

		//execute a request

	}else if (request1 == "EXECUTE") {

		//check request is a win.valid program
		var mdcmd = request2;
		if (mdcmd == "") {
//badproxy:
			USER3 = "Error: " ^ mdcmd.quote() ^ " proxy is not available";
			return;
		}
		mdcmd ^= "PROXY";
//		if (!xx.read(md, mdcmd))
//			goto badproxy;
//		routineexists=routines.count(mdcmd.towstring());
//		MVRoutine *routine;
//		if (!routineexists)
//			goto badproxy;
//		routine=routines[mdcmd.towstring()];
		//newfilename=routine->execute("GETALIAS");

		//provide an output file for the program to be executed
		//nb response file name for detaching processes
		//will be obtained from the output file name Server2 respond
		//this could be improved to work
		var printfilename = linkfilename2;
		var tt = var().oscwd();
		tt.splicer(-7, 7, "");
		if (printfilename.substr(1, tt.length()) == tt)
			printfilename.splicer(1, tt.length(), "..\\");

		//t=printfilename[-1,'b.']
		tt = printfilename.field2(".", -1);
		printfilename.splicer(-tt.length(), tt.length(), "htm");
		mv.SYSTEM.r(2, printfilename);
		if (tracing)
			std::wcout << "Waiting for output ... ";

		//switch to server mode and html output
		var s6 = mv.SYSTEM.a(6);
		var s7 = mv.SYSTEM.a(7);
		var s11 = mv.SYSTEM.a(11);
		mv.SYSTEM.r(6, 1);
		mv.SYSTEM.r(7, 1);
		mv.SYSTEM.r(11, 1);

		//execute the program
		//print timedate2():' starting ':indata[1,60]
		//call mv.msg2('processing remote request','ub',buffer,'')
		USER3 = "OK";
		win.valid = 1;
		USER4 = "";
		//call @mdcmd(field(request,fm,3,99999),iodat,_USER3)
		//request, iodat and response are now passed and returned in @user0,1 and 3
		//other messages are passed back in @user4
		//execute instead of call prevents program crashes from crashing Server
		//USER0 = USER0.field(FM, firstrequestfieldn + 2, 99999);

		//pass the output file in linkfilename2
		//not good method, pass in system?
		if (var("LIST" _VM_ "SELECTBATCHES").locate(USER0.a(1),xx,1))
			USER1 = linkfilename2;
		if ((USER0.a(1)).substr(1, 4) == "VAL.")
			USER1 = linkfilename2;

		//TODO
		//mdcmd.execute();
//		routine->execute(mdcmd);
		//call @mdcmd
		//send errors to neosys
//		USER4="NOT IMPLEMENTED YET:\rEXECUTE " ^ mdcmd ^ "\r" ^ USER0;
		perform(mdcmd);
		//discard any stored input
		mv.DATA = "";

		//call mv.msg2('','db',buffer,'')
		//print timedate2():' finished ':indata[1,60]

		//send errors to neosys
		if (USER4.index("An internal error", 1))
			mv.sysmsg(USER4);

		USER4.cropper();
		USER3.cropper();

		if (USER4) {
			USER1 = "";
			USER3 = "Error: " ^ USER4;
			gosub formatresponse();
		}

		if (USER3 == "") {
			USER3 = "Error: No OK from " ^ mdcmd ^ " " ^ USER0;
			gosub formatresponse();
		}

		//switch off server mode
		mv.SYSTEM.r(6, s6);
		mv.SYSTEM.r(7, s7);
		mv.SYSTEM.r(11, s11);

		//get the printfilename in case the print program changed it
		printfilename = mv.SYSTEM.a(2);
		if (tracing) {
			//print ' got it'
			std::wcout << mv.at(0)<< mv.at(-4);
		}

		//make sure that the output file is closed
		if (printfilename.osopen())
			printfilename.osclose();

	}else if (request1 == "STOPDB") {

		if (globalend.osfile()) {
			USER3 = "Error: Database already stopped/stopping";

		}else{

			var("").oswrite(globalend);

			timex = var().time();
			while (true) {
			//BREAK;
			if (!(mv.otherusers("").a(1) && (var().time() - timex).abs() < 30)) break;;
				var().ossleep(1000);
			}//loop;

			USER1 = "";

			if (mv.otherusers("").a(1)) {
				USER3 = "Error: Could not terminate " ^ mv.otherusers("") ^ " users|" ^ mv.otherdatasetusers("*");
				globalend.osdelete();
			}else{
				var("NET STOP NEOSYSSERVICE").osshell();

				if (request2 == "RESTART") {
					globalend.osdelete();
					var("NET START NEOSYSSERVICE").osshell();
				}
				USER3 = "OK";
			}

		}

	}else if (request1 == "RESTART") {
		USER1 = "";
		USER3 = "OK";

	}else if (request1 == "BACKUP") {
		gosub getbakpars();

		//force additional processes ... that should fail due to backup lock
		(inpath ^ serverflagfilename).osdelete();

		//backup will respond to user itself if it starts
		USER4 = "";
		perform("FILEMAN BACKUP " ^ datasetcode ^ " " ^ bakdisk);

		//if backup has already responded to user
		//then quit and indicate to calling program that a backup has been done
		//user will be emailed
		if (mv.SYSTEM.a(2) == "") {
			mv.PSEUDO = "BACKUP2";
			if (USER4)
				var().stop();
		}

		//note: if backup did not respond already then the requestexit will
		//respond as usual with the error message from backup
		USER1 = "";

	}else if (request1 == "VERSION") {
		USER1 = "";
		USER3 = "UNKNOWN VERSION";

	}else if (request1 == "INSTALL") {
		mv.DATA ^= var().chr(13);
		if (request2 == "")
			request2 = ACCOUNT;
		("INSTALL " ^ request2 ^ " I !:").execute();
		USER3 = USER4;

		//or give an error message

	}else if (1) {
		USER1 = "";
		USER3 = "Error: " ^ USER0.field(FM, 4, 9999).quote() ^ " unknown request";
	}

	return;

}

subroutine geterrorresponse()
{
	var fileerror = mv.FILEERROR;
	USER3 = "Error: FS" ^ fileerror.a(1, 1);//.xlate("SYS_MESSAGES", 11, "X");
//	USER3.swapper("%1%", mv.handlefilename(fileerror.a(2, 1)));
	USER3.swapper("%2%", fileerror.a(2, 2));
	gosub formatresponse();
	return;

}

subroutine formatresponse()
{

	//trim everything after <esc> (why?)
	var tt = USER3.index("<ESC>", 1);
	if (tt)
		USER3 = USER3.substr(1, tt - 1);

	//cannot remove since these may be proper codepage letters
	USER3.converter("|", FM);
	USER3.converter(VM, FM);
	if (USER3.substr(1, 1) == FM)
		USER3.splicer(1, 1, "");
	USER3.swapper(FM, var().chr(13) ^ var().chr(10));

	return;

}

subroutine lockit()
{
	state=1;
return;
	//attempt to lock the record
	//BYPASS ordinary lock,file,key process otherwise
	//the lock record will be checked - and in this case
	//we our own lock record to be present
	code = 5;
	//lockit2:
	var nextbfs = "";
	var handle = file;
	//<arev>
	
	//handle = handle.substr(-1, "B" ^ VM);
	handle = handle.field2(VM,-1);
	//</arev>
	var keyorfilename = keyx;
	var fmc = 2;
	gosub lockit2();
	return;
}

subroutine lockit2()
{
	//rtp57(code, nextbfs, handle, keyorfilename, fmc, record, state);
	std::wcout<<"MFS file handle not handled yet"<<std::endl;
	state=1;
	return;

	state=0;
	if (code==5) state=file.lock(keyx);
	else if (code==6)
	{
		 file.unlock(keyx);
		 state=1;
	}
	return;

}

subroutine unlockit()
{
	return;
	//unlock file,keyx
	code = 6;
	gosub lockit2();
	return;
}

subroutine gettimeouttime()
{
	if (!timeoutsecs) {
		timeouttime = "";
		return;
	}

	var timeoutdate = var().date();
	timeouttime=var().ostime();
	timeouttime += timeoutsecs;
	if (timeouttime > 24 * 60 * 60) {
		timeoutdate += 1;
		timeouttime -= 24 * 60 * 60;
	}
	timeouttime = (timeoutdate ^ "." ^ (timeouttime.floor()).oconv("R(0)#5")) + 0;
	return;
}

subroutine properlock()
{
	//must lock it properly otherwise indexing will try to lock it and fail
	//because it is only in the locks file and not properly locked
	win.valid = 1;

	//must exclude the shadowing system otherwise the entry in the locks file
	//also in balances
	//will cause the lock statement to fail
	var srcfile2 = win.srcfile;
	//< arev >;
	if (!win.srcfile.unassigned())
		srcfile2.swapper("SHADOW.MFS" ^ SVM, "");
	//</arev>

	//dont pass the filename because that causes persistent lock checking
	//in jbase version of mv.lockrecord()
	if (!(mv.lockrecord("", srcfile2, keyx))) {
		if (mv.STATUS != 1) {
			win.valid = 0;
			USER3 = "Error: " ^ keyx.quote() ^ " CANNOT BE WRITTEN BECAUSE IT IS LOCKED ELSEWHERE";
		}
	}

	return;
}

subroutine properunlock()
{
	//nb i think that shadow.mfs is not programmed to remove the locks file entry

	//must unlock it properly otherwise indexing will try to lock it and fail
	//because it is only in the locks file and not properly locked

	//must exclude the shadowing system otherwise the entry in the locks file
	//will be removed as well
	var srcfile2 = win.srcfile;
	//<arev>
	srcfile2.swapper("SHADOW.MFS" ^ SVM, "");
	//</arev>

	srcfile2.unlock( keyx);

	return;

}

subroutine lock()
{
	/////

	//cannot do update security check here, have to do it after
	// we know if the record exists or not
	//security check - cannot lock so cannot write or delete

	lockauthorised = 1;

	//lockdurationinmins is the number of minutes to retain the lock
	if (lockdurationinmins.unassigned())
		lockdurationinmins = 10;
	if (!lockdurationinmins.isnum())
		lockdurationinmins = 10;
	if (!lockdurationinmins)
		lockdurationinmins = 10;

	//sessionid which if relocking must match the one on the lock rec
	//otherwise it is assumed that somebody else has taken over the lock
	//if sessionid is blank then a new session id is created and returned in response
	if (request1 == "RELOCK") {
		newsessionid = sessionid;
	}else{
		newsessionid = "";
		for (int ii = 1; ii <= 8; ii++)
			newsessionid ^= var("01234567890ABDCEF").substr(var(16).rnd() + 1, 1);
	}

	if (!file.open(filename)) {
		gosub badfile();
		return;
	}

	USER3 = "";
	if (request1 == "RELOCK") {
		gosub lockit();
	}else{
		if (mv.lockrecord(filename, file, keyx)) {
			state = 1;
		}else{
			state = 0;
		}
	}
	if (!state) {
		USER3 = "NOT OK";
		return;
	}

	//check locks file
	gosub getostime();
	lockkey = filename ^ "*" ^ keyx;
	if (lockrec.read(locks, lockkey)) {

		if (lockrec.a(5) != newsessionid) {

			//fail if other lock has not timed out
			if (lockrec.a(1) > ostimenow) {
				USER3 = "NOT OK";
				goto lockexit;
			}

			//other lock has timed out so ok
			//no need to delete as will be overwritten below
			//delete locks,lockkey

		}

		//our own session ... so must be relocking (to extend timeout)

	}else{

		if (request1 == "RELOCK") {
			//nb the word "expired" is a key word used in _formtriggers.htm
			USER3 = "Error: Your lock expired and somebody else updated";
			goto lockexit;
		}

		//no lock record so ok

	}

	USER3 = "OK";

	//convert minutes to fraction of one day (windows time format)
	lockduration = lockdurationinmins / (24 * 60);

	//write the lock in the locks file
	lockrec = "";
	lockrec.r(1, lockduration + ostimenow);
	lockrec.r(2, ostimenow);
	lockrec.r(3, connection ? connection.a(1, 2): mv.STATION);
	lockrec.r(4, USERNAME);
	lockrec.r(5, newsessionid);
	mv.FILEERRORMODE = 1;
	mv.FILEERROR = "";
	USER3 = "OK";
	lockrec.write(locks, lockkey);
	if (mv.FILEERROR) {
		mv.mssg("CANNOT WRITE LOCKS RECORD " ^ lockkey);
		gosub geterrorresponse();
	}
	relistlocks = 1;

	sessionid = newsessionid;

lockexit:

	//unlock file,keyx
	if (request1 == "RELOCK") {
		gosub unlockit();
	}else{
		file.unlock( keyx);
	}

	return;

}

subroutine badfile()
{
	////////
	USER3 = "Error: " ^ filename.quote() ^ " file does not exist";
	return;
}

subroutine unlock()
{
	///////

	//sessionid is used as a check that only the locker can unlock
	if (!file.open(filename)) {
		gosub badfile();
		return;
	}

	//lock file,keyx
	gosub lockit();
	if (!state) {
		//zzz perhaps should try again a few times in case somebody else
		//is trying to lock but failing because of our remote lock
		USER3 = "Error: Somebody else has locked the record";
		return;
	}

	//get the current lock else return ok
	lockkey = filename ^ "*" ^ keyx;
	mv.FILEERRORMODE = 1;
	mv.FILEERROR = "";
	if (!lockrec.read(locks, lockkey))
		lockrec = "";
	if (!lockrec) {
		if (mv.FILEERROR.a(1) == 100) {
			//lock is missing but ignore it
			//because we are unlocking anyway
			USER3 = "OK";
		}else{
			gosub geterrorresponse();
		}
		goto unlockexit;
	}

	//check that the current lock agrees with the session id provided
	if (!(sessionid == lockrec.a(5))) {

		//cannot unlock because the lock belongs to somebody else
		USER3 = "Error: Cannot unlock - ";
		if (sessionid == "") {
			USER3 ^= "missing session id";
		}else{
			USER3 ^= "wrong session id";
		}

		goto unlockexit;
	}

	//delete the lock
	mv.FILEERRORMODE = 1;
	mv.FILEERROR = "";
	USER3 = "OK";
	locks.deleterecord(lockkey);
	if (mv.FILEERROR) {
		mv.mssg("CANNOT DELETE LOCK KEY " ^ lockkey);
		gosub geterrorresponse();
	}
	relistlocks = 1;

unlockexit:

	gosub unlockit();

	return;

}
subroutine getsecurity()
{
	if (!mv.SECURITY.read(mv.DEFINITIONS, "SECURITY"))
	{
		//std::wcout<<"CANNOT READ SECURITY"<<std::endl;
		//return;
		mv.SECURITY="";
	}
	//mv.SECURITY.inverter();
	return;

}

subroutine getostime()
{
	ostimenow=var().ostime();
	//convert to windows based date/time (ndays since 1/1/1900)
	//31/12/67 in rev date() format equals 24837 in windows date format
	ostimenow = 24837 + var().date() + ostimenow / 24 / 3600;
	return;

}

subroutine flagserveractive()
{

	//flag that this dataset is being served ("listened") (needed for old mac)
	//does not seem to cause any filesharing errors (stress tested at max speed)
	var("").oswrite(inpath ^ serverflagfilename);

	return;

}

subroutine getbakpars()
{
	//backup params
	//1=date last attempted
	// *2= not used ... was workstation for backup (blank defaults to server)
	//3=min time of day for backup to start (defaults to 03:00)
	//4=max time of day for backup to start (defaults to 4:00)
	//5=days to do backup 12345=sun/mon/tue/wed/thu night!!!
	//2/3/4/5 could be multivalued in parallel (to be programmed)
	//6=tech support email addresse(s) separated by ';'
	//7=backup disk letter
	//8=mv list of datasets not to backup
	//9=1 disable backup
	//10=email addresses to send backup.zip to or replacement for 6 if 6 missing
	//backups@neosys.com is excluded in net from backup.zip recipients

	//determine if backup required
	var bakpars;
	if (!bakpars.read(mv.DEFINITIONS, "BACKUP"))
		bakpars = "";
	var tt;
	if (tt.osread("BACKUP.CFG")) {
		for (int ii = 1; ii <= 99; ii++) {
			if (tt.a(ii))
				bakpars.r(ii, tt.a(ii));
		};//ii;
	}
	var lastbakattemptdate = bakpars.a(1);
	//if bakpars<2> then
	// bakreq=(trim(@station)=bakpars<2>)
	//end else
	// bakreq=onserver
	// end
	var testdata = 0;
	if (datasetcode.substr(-4, 4) == "TEST")
		testdata = 1;
	if (mv.SYSTEM.a(23).ucase().index("TRAINING", 1))
		testdata = 1;
	if (mv.SYSTEM.a(23).ucase().index("TESTING", 1))
		testdata = 1;
	if (bakpars.locate(datasetcode, xx, 8))
		testdata = 1;
	//testdata will not backup but will terminate if bakreq
	//bakreq=not(testdata)
	var bakreq = 1;
	if (bakpars.a(9))
		bakreq = 0;
	//address to email backup is bakpars<10>

	var minbaktime = bakpars.a(3);
	if (minbaktime == "")
		minbaktime = var("2:00").iconv("MT");
	//if @username='neosys' then minbaktime=iconv('00:00','mt')
	var maxbaktime = bakpars.a(4);
	//if maxbaktime='' then maxbaktime=iconv('2:05','mt')
	if (maxbaktime == "")
		maxbaktime = minbaktime + 60 * 10;
	//if @username='neosys' then maxbaktime=iconv('23:59','mt')

	var bakdows = bakpars.a(5);
	if (bakdows == "")
		bakdows = "1234567";

	var bakdisk = bakpars.a(7);

	return;
}

subroutine convlogx()
{

	//assumes at least 0-31 and 249-255 encoded like $hh

	//reserve/use special characters for field separators
	//not really necessary in unicode?
	logx.swapper("^", "%5E");
	logx.swapper("]", "%5D");
	logx.swapper("\\", "%5C");
	logx.swapper("[", "%5B");
	logx.swapper("%FE", "^");
	logx.swapper("%FD", "]");
	logx.swapper("%FC", "\\");
	logx.swapper("%FB", "[");
	logx.swapper(RM, "%FF");
	//swap fm with '^' in logx
	//swap vm with ']' in logx
	//swap sm with '\' in logx
	//swap tm with "[" in logx
	//convert fefdfcfb to '^]\[' in logx
	logx.converter(fefdfcfb, "^]\\[");
	logx.swapper(STM, "%FA");
	logx.swapper(SSTM, "%F9");

	logx.swapper("%20", " ");
	//does not seem to format in xml
	//swap '%20' with '&nbsp;' in logx

	logx.swapper("&", "&amp;");
	logx.swapper(DQ, "&quot;");
	logx.swapper("<", "&lt;");
	logx.swapper(">", "&gt;");
	logx.swapper("%26", "&amp;");
	logx.swapper("%22", "&quot;");
	logx.swapper("%3C", "&lt;");
	logx.swapper("%3E", "&gt;");

	return;
}

subroutine writelogx()
{
	gosub convlogx();
	gosub writelogx2();
	return;
}

subroutine writelogx2()
{
	mv.osbwritex(logx, logfilename, logfilename, logptr);
	logptr += logx.length();
	logx = "";
	return;
}

subroutine writelogx3()
{
	mv.osbwritex(logx, logfilename, logfilename, logptr);
	logx = "";
	return;
}


function filesecurity(in secmode)
{
	if (mv.authorised(filename2 ^ " " ^ secmode, msg0, "")) {
		positive = "";
	}else{
		positive = "#";
	}
	var msgx="";
	if (!(mv.authorised(positive ^ filename2 ^ " " ^ secmode ^ " " ^ keyx.quote(), msgx, ""))) {
		if (positive) {
			msg0.transfer(USER3);
		}else{
			msgx.transfer(USER3);
		}
		gosub formatresponse();
		return 0;
	}
	return 1;

}

subroutine respond()
{
	//method to allow batch programs to respond and detach before finishing

	//determine the responsefile name from the printfilename
	//responsefilename=system<2>
	//if responsefilename else return
	//t=field2(responsefilename,'.',-1)
	//responsefilename[-len(t),len(t)]='3'

	//linkfilename3
	responsefilename = mv.PRIORITYINT.a(100);
	if (!responsefilename)
		return;

	//detach the calling process
	USER3 = request2;
	USER3.converter(VM ^ VM, FM ^ FM);
	USER3.swapper(FM, "\r\n");

	USER3.oswrite(responsefilename);
	//osclose responsefilename

	var().ossleep(2000);

	//indicate that response has been made
	mv.SYSTEM.r(2, "");

	return;

}

subroutine getindexvalues()
{

	return;

/* NOT IMPLEMENTED YET - REQUIRES DB SIDE CODING

 	filename=request1;
 	fieldname=request2;
 	prefix=request3;
 	sortby=request4;

	_USER1 = "";
	if (sortby) {
		if (!(var("AL" _VM_ "AR" _VM_ "DL" _VM_ "DR").locate(sortby, xx))) {
			USER3 = "Invalid sortby " ^ sortby.quote() ^ " in Server,GETINDEXVALUES";
			return;
		}
	}

	//security check
	var temp = filename;

	//zzz
	if (temp == "BATCHES")
		temp = "JOURNAL";

	temp.convert(".", " ");
	temp = mv.singular(temp);
	if (!(mv.authorised(temp ^ " ACCESS", USER4, ""))) {
		USER3 = USER4;
		return;
	}
	if (!(mv.authorised(temp ^ " LIST", USER4, ""))) {
		USER3 = USER4;
		return;
	}

	collectixvals(filename, fieldname, prefix);
	mv.PSEUDO.transfer(USER1);
	if (USER1.substr(1, 1) == FM)
		USER1.splicer(1, 1, "");
	USER3 = "OK";

	if (sortby && USER1) {
		USER1.convert(FM, mv.RM);
		USER1 ^= mv.RM;
		v119("S", "", sortby.substr(1, 1), sortby.substr(2, 1), USER1, flag);
		USER1.convert(mv.RM, FM);
		USER1.splicer(-1, 1, "");
	}

	if (USER0.index("XML", 1)) {
		if (USER1) {
			USER1.swap(FM, "</" ^ fieldname ^ ">" "</record>" ^ (var().chr(13) ^ var().chr(10)) ^ "<record><" ^ fieldname ^ ">");
			USER1.splicer(1, 0, "<record><" ^ fieldname ^ ">");
			USER1 ^= "</" ^ fieldname ^ ">" "</record>";
		}
		USER1.splicer(1, 0, "<records>");
		USER1 ^= "</records>";
	}else{
		//convert fm to vm in iodat
	}

	return;

*/

}


subroutine select()
{

	//USER1=''

//TODO
//	var().clearselect();

	var filename0 = request2;
	var filename = filename0.field(" ", 1, 1);
	var sortselect = request3;
	var dictids = request4;
	var options = request5;

	var file;
	if (!file.open(filename)) {
		USER3 = "Server::select " ^ filename.quote() ^ " cannot open file";
		return;
	}

	//security check
	if (!(var("MENUS" _VM_ "ADMENUS").locate(filename, xx))) {
		var temp = filename;
		temp.convert(".", " ");
		temp = mv.singular(temp);
		if (!(mv.authorised(temp ^ " ACCESS", USER4, ""))) {
			var msgx="";
			if (!(mv.authorised("!#" ^ temp ^ " ACCESS PARTIAL", msgx, ""))) {
				USER3 = USER4;
				return;
			}
		}
		USER4 = "";
	}

	//any data passed to select is assumed to be a selectlist

	if (USER1) {
//TODO:
USER3="GeneralProxy: MAKELIST not implemented yet";
return;
//		makelist("", USER1, "", "");
		sortselect ^= "%SELECTLIST%";
		USER1 = "";
	}

//TODO:
//USER3="GeneralProxy: SELECT2 not implemented yet";
//return;
	call select2(filename0, mv.SYSTEM.a(2), sortselect, dictids, options, USER1, USER3, "", "", "");

	if (USER4) {
		USER3 = USER4.a(1).trim(FM);
	}else{
		USER1 = "%DIRECTOUTPUT%";
		//USER3='ok'
	}

	return;

};

subroutine deleteoldfiles(in inpath, in pattern)
{

	var deletetime = var().date() * 24 * 60 * 60 + var().time() - ageinsecs;

	//for each suitable file
	var filenames=var().oslistf(inpath,pattern);

	while (filenames) {

		//get the file time
		var filename = filenames.field(FM, 1, 1);
		filenames.splicer(1,filename.length()+1,"");
		filename = inpath ^ filename;

		if (!(var(".JPG" _VM_ ".GIF").locate(filename.substr(-4, 4).ucase(),xx,1))) {

			//a file ending .4 means that the .2 and .3 files need to be deleted
			if (filename.substr(-2, 2) == ".4") {
				filename.osdelete();
				filename.splicer(-1, 1, "2");
				filename.osdelete();
				filename.splicer(-1, 1, "3");
				filename.osdelete();

			}else{
				//and delete it if older than the cut off time
				//and has a file extension (ie leave params and params2)
				var fileattributes = filename.osfile();
				var filetime = fileattributes.a(2) * 24 * 60 * 60 + fileattributes.a(3);
				if ((filename.substr(-4, 4)).index(".", 1) && filetime <= deletetime) {
					filename.osdelete();
				}else{
				}
			}

		}

	}//loop;

	return;

}

programexit()

