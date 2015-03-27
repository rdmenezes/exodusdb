For example, classic Multivalue Basic

> PRINT OCONV(FIELD(XX`<6>`,'/',2),'D2'))

using Exodus OO syntax becomes:

> xx.extract(6).field("/",2).oconv("D2").outputl();
or
> printl(xx.extract(6).field("/",2).oconv("D2"));

Many people find the OO method of chaining operations from left to right, and with the parameters next to their function name, to be very readable. In the classic multivalue basic example above, the "OCONV" is quite far away from its parameter 'D2' so requiring quite a lot of mental parsing.

The .extract(6) instead of `<6>` is rather ugly and it might be changed. Likewise outputl() might become something shorter and sweeter.

#### Function Types ####

|TYPE	|FUNCTION TYPE|
|:----|:------------|
|var=	|traditional functions that return values|
|if	|traditional conditional statements that started with if or ended with then or else (or could have)|
|cmd	|traditional commands with no outputs|
|cmd2	|traditional commands that had parameters on the left hand side of the = assigments (or -er functions)|

#### Parameters Types ####

|in| Parameters that provide data to the function. Can be variables or raw data like 1 or "X"|
|:-|:----------------------------------------------------------------------------------------|
|io| Parameters that may provide and/or return data. Must be variables not raw data like 1 or "X"|
|out| Parameters that return data. Must be variables not raw data like 1 or "X"|
|blank|The default. Parameters that provide data to the function. Can be variables or raw data like 1 or "X"|

Optional Parameters

|Key	|Default|
|:---|:------|
|=""	|""|
|=" "	|" "|
|="."	|"."|
|=1	|1 |
|=0	|0 |
|=true	|true|
|=false	|false|

#### Complete List of Functions ####

Time/Date/Sleep
|var=	|var.date()|
|:----|:---------|
|var=	|var.time()|
|var=	|var.timedate()|
|cmd	|var.ossleep(milliseconds)|
|var=	|var.ostime()|

System File/Directory
|if	|var.osopen(osfilename)|
|:--|:---------------------|
|cmd2	|var.osbread(filehandle, startoffset, length)|
|cmd	|var.osbwrite(filehandle, startoffset)|
|cmd	|var.osclose()|
|if	|var.osread()|
|if	|var.oswrite(osfilename)|
|if	|var.osdelete()|
|if	|var.osdelete(osfilename)|
|if	|var.osrename(newosfilename)|
|if	|var.oscopy(to\_osfilename)|
|var=	|var.oslist(path=".", wildcard="", mode=0)|
|var=	|var.oslistf(path=".", wildcard="")|
|var=	|var.oslistd(path=".", wildcard="")|
|var=	|var.osfile()|
|var=	|var.osdir()|
|if	|var.osmkdir()|
|if	|var.osrmdir(bool evenifnotempty=false)|
|var=	|var.oscwd()|
|var=	|var.oscwd(path)|
|cmd	|var.osflush()|

Program Control
|if	|var.load(libraryname)|
|:--|:--------------------|
|var=	|var.call(libraryname, functionname)|
|var=	|var.call(functionname)|
|var=	|var.call()|
|var=	|var.suspend()|
|var=	|var.osshell()|
|if	|var.osgetenv(name)|
|if	|var.ossetenv(name)|
|cmd	|var.stop(text="")|
|cmd	|var.abort(text="")|
|var=	|var.perform()|
|var=	|var.execute()|
|var=	|var.chain()|
|var=	|var.logoff()|
|var=	|var.debug()|
|cmd	|var.breakon()|
|cmd	|var.breakoff()|

Standard Output
|cmd2	|var.output()|
|:----|:-----------|
|cmd2	|var.outputl()|
|cmd2	|var.outputtab()|
|cmd2	|var.output(var1)|
|cmd2	|var.outputl(var1)|
|cmd2	|var.outputtab(var1)|
|cmd2	|var.logput()|
|cmd2	|var.logputl()|
|cmd2	|var.logput(var1)|
|cmd2	|var.logputl(var1)|
|cmd2	|var.errput()|
|cmd2	|var.errputl()|
|cmd2	|var.errput(var1)|
|cmd2	|var.errputl(var1)|
|cmd2	|var.put(std::ostream &ostream1)|

Cursor
|var=	|var.at(columnorcode)|
|:----|:-------------------|
|var=	|var.at(column, row)|
|var=	|var.getcursor()|
|cmd	|var.setcursor()|
|var=	|var.getprompt()|
|cmd	|var.setprompt()|

Standard Input
|if	|var.input()|
|:--|:----------|
|if	|var.input(prompt)|
|if	|var.inputn(n)|
|if	|var.eof()|

Variable Control
|if	|var.assigned()|
|:--|:-------------|
|if	|var.unassigned()|
|cmd2	|var.transfer(OUT mv2)|
|cmd2	|var.exchange(OUT mv2)|
|var=	|var.addressof()|
|cmd	|var.clear()|
|cmd	|var.clearcommon()|

Numeric
|var=	|var.bitnot()|
|:----|:-----------|
|var=	|var.abs()|
|var=	|var.pwr(exponent)|
|var=	|var.rnd()|
|cmd	|var.initrnd()|
|var=	|var.exponent()|
|var=	|var.sqrt()|
|var=	|var.sin()|
|var=	|var.cos()|
|var=	|var.tan()|
|var=	|var.atan()|
|var=	|var.loge()|
|var=	|var.integer()|
|var=	|var.floor()|
|var=	|var.round(ndecimals=0)|

String Creation
|var=	|var.chr()|
|:----|:--------|
|var=	|var.chr(num)|
|var=	|var.str(num)|
|var=	|var.space()|

String Info
|if	|var.match(matchstr, options="")|
|:--|:------------------------------|
|var=	|var.seq()|
|var=	|var.dcount(substrx)|
|var=	|var.count(substrx)|
|var=	|var.length()|
|var=	|var.len()|
|	 |data()|
|if	|var.isnum()|
|if	|var.isalpha()|

String Manipulation
|cmd2	|var.converter(fromchars, tochars)|
|:----|:--------------------------------|
|cmd2	|var.swapper(fromstr, tostr, options="")|
|cmd2	|var.splicer(start1, length, str)|
|cmd2	|var.quoter()|
|cmd2	|var.unquoter()|
|cmd2	|var.ucaser()|
|cmd2	|var.lcaser()|
|cmd2	|var.inverter()|
|cmd2	|var.trimmer(trimchar=" ")|
|cmd2	|var.trimmerf(trimchar=" ")|
|cmd2	|var.trimmerb(trimchar=" ")|
|cmd2	|var.trimmer(trimchar)|
|cmd2	|var.trimmerf(trimchar)|
|cmd2	|var.trimmerb(trimchar)|
|cmd2	|var.fieldstorer(sepchar, fieldn, nfields, replacement)|
|cmd2	|var.substrer(startx)|
|cmd2	|var.substrer(startx, length)|

String Filter
|var=	|var.convert(fromchars, tochars)|
|:----|:------------------------------|
|var=	|var.swap(fromstr, tostr, options="")|
|var=	|var.splice(start1, length, str)|
|var=	|var.quote()|
|var=	|var.unquote()|
|var=	|var.ucase()|
|var=	|var.lcase()|
|var=	|var.invert()|
|var=	|var.trim(trimchar=" ")|
|var=	|var.trimf(trimchar=" ")|
|var=	|var.trimb(trimchar=" ")|
|var=	|var.trim(trimchar)|
|var=	|var.trimf(trimchar)|
|var=	|var.trimb(trimchar)|
|var=	|var.fieldstore(sepchar, fieldn, nfields, replacement)|
|var=	|var.hash()|

String Extraction
|var=	|var.substr(startx)|
|:----|:-----------------|
|var=	|var.substr(startx, length)|
|var=	|var.index(substr, startchar1=1)|
|var=	|var.field(substrx, fieldnx, nfieldsx=1)|
|var=	|var.field2(substrx, fieldnx, nfieldsx=1)|

ICONV/OCONV
|var=	|var.oconv(convstr)|
|:----|:-----------------|
|var=	|var.iconv(convstr)|

Dynamic Array
|cmd	|var.matparse(OUT varray1)|
|:---|:------------------------|
|var=	|var.lower()|
|var=	|var.raise()|
|var=	|var.crop()|
|cmd2	|var.lowerer()|
|cmd2	|var.raiser()|
|cmd2	|var.cropper()|
|var=	|var.remove(OUT startx, OUT delimiterno)|
|var=	|var.replace(fieldn, valuen, subvaluen, replacement)|
|var=	|var.extract(fieldn, valuen=0, subvaluen=0)|
|var=	|var.insert(fieldn, valuen, subvaluen, insertion)|
|var=	|var.erase(fieldn, valuen=0, subvaluen=0)|
|cmd2	|var.replacer(fieldn, valuen, subvaluen, replacement)|
|cmd2	|var.inserter(fieldn, valuen, subvaluen, insertion)|
|cmd2	|var.eraser(fieldn, valuen=0, subvaluen=0)|
|if	|var.locate(target, OUT setting, fieldn=0, valuen=0)|
|if	|var.locateby(target, ordercode, OUT setting, fieldn=0, valuen=0)|
|if	|var.locateby(target, ordercode, OUT setting, fieldn=0, valuen=0)|
|if	|var.locateusing(target, usingchar, OUT setting, fieldn=0, valuen=0, subvaluen=0)|
|if	|var.locateusing(target, usingchar)|
|var=	|var.sum(sepchar=VM_)_|

Database
|if	|var.connect(connectioninfo="")|
|:--|:-----------------------------|
|if	|var.disconnect()|
|if	|var.createdb(dbname, errmsg)|
|if	|var.deletedb(dbname, errmsg)|
|if	|var.begintrans()|
|if	|var.rollbacktrans()|
|if	|var.committrans()|
|if	|var.createfile(filename, options="")|
|if	|var.deletefile()|
|if	|var.clearfile()|
|var=	|var.listfiles()|
|if	|var.createindex(fieldname, dictfile="")|
|if	|var.deleteindex(fieldname)|
|var=	|var.listindexes(filename)|

Files and Records
|if	|var.open(dbfilename)|
|:--|:-------------------|
|if	|var.open(dict, dbfilename)|
|cmd	|var.close()|
|if	|var.select(sortselectclause="")|
|cmd	|var.clearselect()|
|if	|var.readnext(OUT key)|
|if	|var.readnext(OUT key, OUT valueno)|
|if	|var.selectrecord(sortselectclause="")|
|if	|var.readnextrecord(OUT key, OUT record)|
|if	|var.lock(key)|
|cmd	|var.unlock(key)|
|cmd	|var.unlockall()|
|if	|var.read(filehandle, key)|
|if	|var.readv(filehandle, key, fieldn)|
|if	|var.write(filehandle, key)|
|if	|var.writev(filehandle, key, fieldn)|
|if	|var.deleterecord(key)|
|if	|var.updaterecord(file, key)|
|if	|var.insertrecord(file, key)|

Dictionary
|var=	|var.calculate()|
|:----|:--------------|
|var=	|var.xlate(filename, fieldno, mode)|
|var=	|var.xlate(filename, fieldno, mode)|

The following is not very informative and might be removed

Automatic Conversions
|if	|var.toBool()|
|:--|:-----------|
|int	|var.toInt()|
|double	|var.toDouble()|

Explicit Conversions
|wstring	|var.towstring()|
|:-------|:--------------|
|string	|var.tostring()|

Initialisation
|var	|var()|
|:---|:----|
|var=	|var|
|var=	|'x'|
|var=	|"xxx"|
|var=	|bool|
|var=	|int|
|var=	|long long|
|var=	|double|
|var=	|(**char, intlength)**|
|var=	|wstring|
|var=	|string|

Assignment to
|int=	|var|
|:----|:--|
|bool=	|var|
|string=||var|
|wstring=||var|

Self Updating
|cmd2	|var^=var|
|:----|:-------|
|cmd2	|var^=int|
|cmd2	|var^=double|
|cmd2	|var^=char|
|cmd2	|var^=**char**|
|cmd2	|var^=wstring|
|var=	|var++int|
|var=	|var--int|
|cmd2	|var++|
|cmd2	|var--|
|cmd2	|var+=var|
|cmd2	|var-=var|