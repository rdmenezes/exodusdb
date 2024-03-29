//Copyrightę 2000 NEOSYS Software Ltd. All Rights Reserved.//**Start Encode**

//constants
///////////

var gmonthnames = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December']
var gweekdaynames = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday']
var gaccount

//functions
///////////

function general_dict_aliassystem(di) {
    di.required = true
    di.unique = true
    di.length = 5
}

function general_dict_aliascode(di) {
    di.required = true
}

function general_list_markets() {
    openwindow('EXECUTE\rAGENCY\rLISTMARKETS')
    return true
}

function general_list_currencies() {
    var ratedate = neosysinput('List of Currencies &amp; Exchange Rates\r\rDate for exchange rates?', neosysdate().neosysoconv('[DATE]'))
    if (!ratedate) return false
    openwindow('EXECUTE\rGENERAL\rLISTCURRENCIES\r' + ratedate)
    return true
}

function general_pop_settings() {

    var arguments = {}

    //pass in the local window settings
    if (typeof finance_getsettings != 'undefined')
        arguments.defaultrevstr = finance_getsettings('string').neosysconvert(':;', fm + vm)
    else
        arguments.defaultrevstr = neosysgetcookie(glogincode, 'NEOSYS2', 'settings').neosysconvert('\\!', fm + vm)

    arguments.close = 'yes'
    var result = neosysshowmodaldialog('../finance/settings.htm', arguments)
    if (!result)
        return false

    //update local copy of settings
    //gfinance_localsettingsstring=finance_getsettingsstring()
    //get the cookie version of the string (to get the local one call getsetting('string'))
    //gfinance_localsettingsstring=finance_getsettings().neosysconvert(fm+vm,':;')
    //if (typeof finance_getsettings!='undefined')
    // gfinance_localsettingsstring=finance_getsettings('string').neosysconvert(':;',fm+vm)
    //else
    // gfinance_localsettingsstring=neosysgetcookie(glogincode,'NEOSYS2','settings').neosysconvert('\\!',fm+vm)
    gfinance_localsettingsstring = result

    //re-store the latest settings
    if (typeof SETTINGS_STRING != 'undefined')
        gds.setx('SETTINGS_STRING', null, gfinance_localsettingsstring.neosysconvert(fm + vm, ':;'))

    //redisplay the latest settings
    if (typeof SETTINGS != 'undefined')
        gds.regetx('SETTINGS')

    //set any scope to settings only
    if (typeof SCOPE != 'undefined') {
        gds.setx('SCOPE', null, 1)
    }

    return result

}

function general_print_account(acno, companycode) {

    db.request = 'EXECUTE\rFINANCE\rLEDGERPRINT'

    if (!acno) acno = gvalue
    if (!companycode && typeof COMPANY_CODE != 'undefined') companycode = gds.getx('COMPANY_CODE')

    if (!acno) return neosysinvalid('Please enter an account number first')

    //the account number
    var dbdata = acno
    //by period and not date
    dbdata = dbdata.neosysreplace(16, 0, 0, '1')
    //the current settings
    var financesettings = finance_getsettings('string').split(':')
    if (companycode) financesettings[3 - 1] = companycode
    dbdata = dbdata.neosysreplace(10, 0, 0, financesettings.join(':'))

    //get the report url
    if (!db.send(dbdata)) return neosysinvalid(db.response)

    //open the report url
    window.open(db.data)

    return true

}

//TODO move to neosys.js
function general_dict_datasetcode(di, many, orcurrent) {
    if (!many) many = false
    if (!orcurrent) orcurrent = false
    di.popup = 'general_pop_datasetcode(' + many + ',' + orcurrent + ')'
    di.validation = 'general_val_datasetcode(' + many + ',' + orcurrent + ')'
    di.length = 8;
}

function general_pop_datasetcode(many, orcurrent) {
    if (!general_getdatasets()) return false

    var popdata = neosyscloneobj(gdatasets)
    if (orcurrent) {
        popdata[0].splice(0, 0, 'Current')
        popdata[1].splice(0, 0, 'CURRENT')
    }

    var cols = [[0, 'Name'], [1, 'Code']]
    var returncoln = 1
    var defaultreply = ''
    var inverted = true
    var datasetcode = neosysdecide('Which ' + 'dataset' + (many ? '(s' : '') + ' do you want?', popdata, cols, returncoln, defaultreply, many, inverted)
    return datasetcode
}

function general_val_datasetcode(many, orcurrent) {

    if (gvalue == 'CURRENT') {
        if (!orcurrent) return neosysinvalid('"CURRENT" is not allowed here')
        return true
    }

    if (!general_getdatasets()) return false

    if (gvalue != 'CURRENT' && gvalue.indexOf('CURRENT') >= 0) return neosysinvalid('You cannot choose CURRENT and other datasets')

    var values = many ? gvalue.split(':') : [gvalue]
    for (var ii = 0; ii < values.length; ii++) {
        if (!gdatasets[1].neosyslocate(values[ii])) return neosysinvalid(values[ii].neosysquote() + ' is not a valid dataset code')
    }

    return true

}

var gdatasets
function general_getdatasets(refresh) {
    if (refresh || !gdatasets) {
        db.request = 'EXECUTE\rGENERAL\rGETDATASETS'
        if (!db.send()) return neosysinvalid(db.response)
        //split inverted
        gdatasets = db.data.neosyssplit(vm + sm, true)
    }
    return gdatasets.length > 0
}

function general_dict_period(di, mode, otherperiodid, conversion) {
    neosysassertobject(di, 'general_dict_period', 'di')
    if (!mode) mode = ''
    if (!otherperiodid) otherperiodid = ''
    if (!conversion) conversion = '[PERIOD_OF_YEAR]'//'[FINANCIAL_PERIOD]'
    if (!di.conversion) di.conversion = conversion
    di.defaultvalue = 'general_def_period("' + mode + '","' + otherperiodid + '")'
    di.validation = 'general_val_period("' + mode + '","' + otherperiodid + '")'
    di.length = 5
}

function general_def_period(mode, otherperiodid) {
    if (gvalue || !otherperiodid) return ''
    otherperiod = gds.getx(otherperiodid)
    if (typeof otherperiod == 'object') otherperiod = otherperiod[0]
    return otherperiod
}

var gmaxperiodno
function general_val_period(mode, otherperiodid) {

    if (!gvalue) return true
    if (gmaxperiodno) gmaxperiodno = 12

    var value = gvalue.split('/')

    var oldvalue = goldvalue.split('/')

    if (mode && otherperiodid) {

        otherperiod = gds.getx(otherperiodid)
        if (typeof otherperiod == 'object') otherperiod = otherperiod[0]
        var othervalue = otherperiod.split('/')

        //get year from the other period
        if (value.length == 1 || !value[1]) {

            //missing year is taken from other period or current period
            if (otherperiod) {
                value[1] = othervalue[1]
                if (mode == 'FROM') {
                    //make sure to month is not less than from month
                    if (value[0] > othervalue[0]) othervalue[0] = value[0]
                }
                else {
                    //make sure from month is not less than to month
                    if (value[0] < othervalue[0]) othervalue[0] = value[0]
                }
            }
            else {
                value[1] = gperiod.split('/')[1]
            }
        }

        var otherjulianperiod = neosysjulianperiod(othervalue.join('/'), gmaxperiodno)
        var julianperiod = neosysjulianperiod(value.join('/'), gmaxperiodno)

        //prevent toperiod less than fromperiod
        if (mode == 'FROM' && julianperiod > otherjulianperiod) othervalue = value
        if (mode == 'UPTO' && julianperiod < otherjulianperiod) othervalue = value

        //if change fromperiod and toperiod blank or same as from goldvalue
        if (mode == 'FROM' && (otherperiod == '' || otherperiod == goldvalue)) othervalue = value

        //update the otherperiod
        if (otherperiod != othervalue.join('/')) gds.setx(otherperiodid, grecn, othervalue.join('/'))

        gvalue = value.join('/')

    }

    return true

}

function general_set_gcompanycode(newcompanycode) {

    //change the environment for this and future windows
    gcompanycode = newcompanycode
    neosyssetcookie(glogincode, 'NEOSYS2', gcompanycode, 'cc')

    //get the new company and currency maybe
    var company = []
    if (!company.neosysread('COMPANIES', gcompanycode)) {
        neosysnote(company.neosysresponse)
    }
    else {
        if (company[30]) {

            gmarketcode = company[30].neosysextract(1, 1)
            neosyssetcookie(glogincode, 'NEOSYS2', gmarketcode, 'mk')

            var market = []
            if (!market.neosysread('MARKETS', gmarketcode)) {
                neosysnote(market.neosysresponse)
            }
            else {
                if (market[5]) {
                    gmaincurrcode = market[5]
                    neosyssetcookie(glogincode, 'NEOSYS2', gmaincurrcode, 'mc')
                }
            }
        }
    }

}

function general_switch_company() {

    var newcompanycode = general_pop_companies()
    if (!newcompanycode) return false
    if (typeof newcompanycode == 'object') newcompanycode = newcompanycode[0]

    general_set_gcompanycode(newcompanycode)

    //try to close current record (to force reissue of key based on company)
    if (typeof closerecord_onclick == 'function' && !closerecord_onclick()) return false

    return true

}

function general_view_log(mode, logdate) {
    if (!logdate) logdate = ''
    return openwindow('EXECUTE\rGENERAL\rVIEWLOG\r' + mode + '\r\r\r' + logdate)
}

function general_clear_log(mode) {

    if (!neosysyesno('Are you SURE that you want to\rirrevocably clear the ' + mode + ' log?')) return false

    db.request = 'EXECUTE\rGENERAL\rCLEARLOG\r' + mode
    db.send()
    alert(db.response)
    return true
}

function general_open_address(addresscode, name) {
    var url = '..\\general\\addresses.htm'
    args = {}
    if (addresscode) {
        args.key = escape(addresscode)
        if (name) args.name = escape(name)
    }
    windowopen(url, args)
    return true
}

//accounts
//////////

function general_dict_acno(di, validaccs, companycodedictid, accounttype, controltype, many) {

    if (!many) many = false///many only if one did in group!(!di.groupno)
    if (!validaccs) validaccs = ''
    if (!companycodedictid) companycodedictid = ''
    if (!accounttype) accounttype = ''
    if (!controltype) controltype = ''
    if (!di.validation) di.validation = ''

    //di.filename='ACCOUNTS'
    temp = ((companycodedictid && typeof companycodedictid == 'string' && companycodedictid.slice(0, 1) != '=') ? ',NOCOMPANY' : '')
    di.conversion = '[ACCOUNT_NO' + temp + ']'
    var params = [validaccs.neosysquote(), companycodedictid.neosysquote(), accounttype.neosysquote(), controltype.neosysquote(), many, di.validation.neosysquote()].join(',')
    di.popup = 'general_pop_accounts(' + params + ')'
    di.validation = 'general_val_account(' + params + ')'
    di.link = 'general_print_account(gvalue)'
    di.length = 10

}

function general_dict_accountname(di, acnoid) {
    //use second subvaluen if available, otherwise the first
    di.functioncode = 'return this.getx("' + acnoid + '").neosysfield(sm,-1).neosysxlate("ACCOUNTS",1)'
}

function general_val_account(validaccs, companycodedictid, accounttype, controltype, many, postvalidation) {

    if (gvalue == '') return true

    //get validaccs if expression
    //not used in this function
    //if (typeof validaccs=='string'&&validaccs.slice(0,1)=='=') validaccs=neosysevaluate(validaccs.slice(1))

    if (typeof validaccs == 'undefined') validaccs = ''
    if (typeof companycodedictid == 'undefined') companycodedictid = ''
    if (typeof accounttype == 'undefined') accounttype = ''

    //get the required company if any
    var enteredcompanycode = gvalue.split(',')[1]
    if (!enteredcompanycode) enteredcompanycode = ''
    var reqcompanycode = enteredcompanycode
    if (!enteredcompanycode && companycodedictid) {
        if (typeof companycodedictid == 'string' && companycodedictid.slice(0, 1) == '=') reqcompanycode = neosysevaluate(companycodedictid.slice(1))
        //else reqcompanycode=gds.getx(companycodedictid)
        //company code may be grouped but in different group so force all with null
        else reqcompanycode = gds.getx(companycodedictid, null)
    }
    //allow for multiple company codes
    if (typeof reqcompanycode != 'string') reqcompanycode = reqcompanycode.join(';')

    //call the database to validate the account
    var acnos = [gvalue]
    gvalue = []
    while (acnos.length) {

        acno = acnos[0]
        acnos = acnos.slice(1)

        db.request = 'EXECUTE\rFINANCE\rFINDACCOUNT\r' + acno + '\r' + accounttype
        if (!db.send()) return neosysinvalid(db.response)

        //if response is an account list then choose an account
        if (db.data.indexOf('/ACCOUNTLIST/') + 1) {
            //var acno=finance_pop_accountlist(db.data,many)
            //this code cannot expand many account numbers so only allow one to be "found"
            var acno = finance_pop_accountlist(db.data, many)
            if (!acno) return false
            //acnos.splice(0,1,acno)
            //doesnt splice properly so abbreviate
            //results in repeated loop and undefined on second pass so change
            //acnos=acno
            //gvalue=[]
            //acnos=[]
            //gvalue=acno
            if (typeof acno == 'string') acnos = [acno]
            else acnos = acno
            gvalue = []
            continue//ie revalidate normally
        }

        //extract the response
        var data = db.data.split(rm)
        gvalue[gvalue.length] = data[0]
        var account = data[1].split(fm)
        var warnings = data[3]

        //any warnings
        if (warnings && !confirm(warnings.neosysconvert(fm, '\r')))
            return false

        //save the accounts in the cache (not needed if from cache already?)
        writecacherecord(account, 'ACCOUNTS')
        var a0 = account[0]
        var a10 = account[10]
        account[0] = '.' + a10
        account[10] = a0
        writecacherecord(account, 'ACCOUNTS')
        account[0] = a0
        account[10] = a10

        //breakup input
        var accno = gvalue[gvalue.length - 1]
        var companycode = accno.neosysextract(1, 1, 1).neosysfield(',', 2)

        if (!companycode && reqcompanycode.indexOf(';') < 0) companycode = reqcompanycode
        if (companycode && companycode != '*') {

            var msg = account[10] + ' - ' + account[1] + '\rAccount belongs to company ' + account[5]

            //check account company
            //if (account[5]&&companycode!=account[5]) return neosysinvalid(msg)
            //if (account[5]&&!(companycode.split(vm).neosyslocate(account[5]))) return neosysinvalid(msg)

            //check required company
            if (reqcompanycode && account[5] && !(reqcompanycode.split(';').join(vm).split(vm).neosyslocate(account[5]))) return neosysinvalid(msg)

            //check input company
            if (companycode && account[5] && !(companycode.split(';').join(vm).split(vm).neosyslocate(account[5]))) return neosysinvalid(msg)

            //check company access
            var tt = account[5]
            if (!tt) tt = companycode
            if (!neosyssecurity('COMPANY ACCESS ' + neosysquote(tt))) return neosysinvalid(gmsg)

        }

        //check master ledger
        if (accounttype == '-' && account[6]) return neosysinvalid(account[1] + ' - ' + account[10] + ' is a subsidiary ledger account\r(not in the master ledger) and cannot be used here.')

        //check not control account
        if (!controltype) {
            if (account[7]) return neosysinvalid(account[1] + ' - ' + account[10] + ' is a control account and cannot be used here.')
        }

        //check is control account
        if (controltype && controltype != '*') {
            if (!account[7]) return neosysinvalid(account[1] + ' - ' + account[10] + ' is not control account and cannot be used here.')
        }

        //gvalue=gvalue.split(sm)[0]

        gaccount = account

    } //while acnos.length

    //optional additional validation function
    if (postvalidation) return eval(postvalidation)

    return true

}

function general_pop_accounts(validaccs, companycodedictid, accounttype, controltype, many) {

    //get validaccs if expression
    if (typeof validaccs == 'string' && validaccs.slice(0, 1) == '=') validaccs = neosysevaluate(validaccs.slice(1))

    //get the required company if any
    var companycode = ''
    if (companycodedictid && typeof companycodedictid == 'string') {
        if (companycodedictid.slice(0, 1) == '=') companycode = neosysevaluate(companycodedictid.slice(1))
        else companycode = gds.getx(companycodedictid)
    }

    return finance_pop_accounts(validaccs, companycode, accounttype, controltype, many)

}

/*

function not_used_ACCOUNT_NO(mode,value,params)
{

//same as ACCOUNT_NO except it validates the account number
//exists on ICONV and adds sm and internal account number
 
//can handle an array of values
if (typeof(value)=='object') return neosysconvarray(ACCOUNT_NO_old,mode,value,params)

if (value=='') return ''
  
//output convertion
if (mode=='OCONV')
{
 
//return the external account number - which is stored in the data as well
//should really look at the internal account number in value.split(sm)[1] and
//get the latest external account number but this is faster to execute
var ovalue=value.split(sm)[0]
if (params=='NOCOMPANY') ovalue=ovalue.split(',')[0]
return ovalue.neosystrimr(',')

}
 
//input conversion
 
if (params=='NOCONVERT') return value
 
//split the external and internal codes
//nb in iconv we do not expect an internal code
//if it is there and not reading accounts file leave it
ivalue=value.split(sm)
ivalue[0]=ivalue[0].split(',')

if (params=='NOCOMPANY') ivalue=[ivalue[0]]
 
//get the internal account number
gaccount=[]
if (!gaccount.neosysread('ACCOUNTS','.'+ivalue[0][0]))
{
if (!gaccount.neosysread('ACCOUNTS',ivalue[0][0]))
{
//neosysquote(value)+' account does not exist.'
gmsg=gaccount.neosysresponse
return null
}
neosysnote('Account '+ivalue[0][0]+' has been changed to '+gaccount[10])
gaccount[0]=ivalue[0][0]
ivalue[0][0]=gaccount[10]
gaccount[10]=gaccount[0]
}

//check company_code
//get the internal account number
if (ivalue[0][1])
{
company=[]
if (!company.neosysread('COMPANIES',ivalue[0][1]))
{
gmsg=company.neosysresponse
return null
}
}
 
//join the company code if any
ivalue[0]=ivalue[0].join(',')

ivalue[1]=gaccount[10]
 
return ivalue.join(sm)
 
}
*/

function ACCOUNT_NO(mode, value, params) {

    //purpose is simply to remove the internal account number
    //during OCONV for display
    //during ICONV it effectively does nothing as there is no iacno

    //account number pattern is:
    //external: externalacno{,companycode)
    //internal: externalacno{,companycode} subvaluemark internalacno

    var result = value.split(sm)[0]

    //option to prevent company code
    if (params == 'NOCOMPANY') result = result.split(',')[0]

    return result

}

function ACCOUNT_NO2(mode, value, params) {

    if (value == '') return value

    //during OCONV change from original account number to current account number
    //during ICONV the reverse
    //if account record cannot be found then dont convert

    var prefix = mode == 'OCONV' ? '' : '.'

    var result = gvalue
    var account = []
    if (account.neosysread('ACCOUNTS', prefix + value))
        result = account[10]

    return result

}

//companies
///////////

function general_dict_companycode(di, many, defaulted) {
    if (!many) many = false
    if (true || many || di.required == false)
        di.popup = 'general_pop_companies(' + many + ')'
    else
        di.dropdown = 'SELECT\rCOMPANIES\r\rCOMPANY_CODE COMPANY_NAME\rXML' + fm + 'COMPANY_CODE\rCOMPANY_NAME'
    di.length = 3
    if (defaulted) di.defaultvalue = 'gcompanycode'
    di.validation = 'general_val_companycode()'
    if (di.groupno) di.unique = true
    if (!gdatafilename || di.fieldno) di.link = 'windowopen2("../general/companies.htm")'
}

function general_val_companycode() {

    if (gvalue == '') return true

    var company = []
    if (!company.neosysread('COMPANIES', gvalue)) {
        return neosysinvalid('Company ' + gvalue.neosysquote() + ' is not on file\ror cannot be accessed.')
    }

    return true

}

function general_dict_companyname(di, companydictid) {
    neosys_dict_text(di)
    di.length = 40
    if (di.type != 'F') {
        if (!companydictid) companydictid = 'COMPANY_CODE'
        di.functioncode = 'return this.getx("' + companydictid + '").neosysxlate("COMPANIES",1)'
    }
}

function general_pop_companies(many) {
    return neosysfilepopup('COMPANIES', [['COMPANY_NAME', 'Company Name'], ['COMPANY_CODE', 'Code'], ['CURRENT_YEAR', 'Year'], ['CLOSED_PERIOD', 'Finance Closed'], ['CLOSED_PERIOD_OPERATIONS', 'Operations Closed']], 1, 'BY-DSND CURRENT_YEAR', many)
}

//markets
/////////

function general_dict_marketcode(di, many) {
    di.filename = 'MARKETS'

    if (typeof many == 'undefined') {
        if (di.groupno && Number(di.groupno))
            many = true
        else
            many = false
    }

    //set the default unless already set
    if (typeof di.defaultvalue == 'undefined')
        di.defaultvalue = neosysquote(general_def_market_code())

    //if required and there is a default then use a dropdown
    if (di.required && di.defaultvalue) {
        di.dropdown = 'SELECT\rMARKETS\r\rCODE NAME\rXML' + fm + 'CODE\rNAME'
        di.length = 20
    }
    else {
        di.popup = 'general_pop_markets(' + many + ')'
        di.length = 3
    }
    if (!gdatafilename || di.fieldno)
        di.link = 'windowopen2("../general/markets.htm")'

}

function general_dict_marketname(di, marketcodeid) {
    if (typeof marketcodeid == 'undefined') marketcodeid = 'MARKET_CODE'
    di.functioncode = 'return this.getx("' + marketcodeid + '").neosysxlate("MARKETS",1)'
    di.length = 30
}

function general_def_market_code() {
    return gmarketcode
}

function general_pop_markets(many, sortselect) {
    if (!sortselect)
        sortselect = ''
    return neosysfilepopup('MARKETS', [['NAME', 'Market Name'], ['CODE', 'Market Code']], 1, sortselect, many)
}

//currencies
////////////

function general_def_currency_code() {
    return gbasecurr
}

function general_val_currencycode(currencycode) {

    if (typeof currencycode == 'undefined')
        currencycode = gvalue

    if (!currencycode)
        return true

    //check exists
    var currency = []
    if (!currency.neosysread('CURRENCIES', currencycode))
        return neosysinvalid(currency.neosysresponse)

    //prevent using stopped currencies
    if (gKeyNodes.length) {
        var currencyname = currency[1].neosysconvert('<>', '()')
        if (currency[25] || currencyname.toUpperCase().indexOf('(STOP)') >= 0)
            return neosysinvalid(currencyname + '\r\rYou cannot use this currency because it is stopped\r\r' + currency[25])
    }

    return true

}

function general_dict_currencycode(di) {
    di.filename = 'CURRENCIES'
    di.length = 3
    di.maxlength = 3

    //if required and there is a default then use a dropdown
    if (di.required && di.defaultvalue) {
        di.dropdown = 'SELECT\rCURRENCIES\r\rCURRENCY_CODE CURRENCY_NAME\rXML' + fm + 'CURRENCY_CODE\rCURRENCY_NAME'
    }
    else {
        di.popup = 'general_pop_currencies()'
    }

    di.link = 'windowopen2("../general/currencies.htm")'

    //set the default unless already set
    if (typeof di.defaultvalue == 'undefined' && !di.unique) di.defaultvalue = neosysquote(general_def_currency_code())

    if (typeof di.validation == 'undefined')
        di.validation = 'general_val_currencycode()'

}

function general_dict_currencyname(di, currencycode_dictid) {
    if (typeof currencycode_dictid == 'undefined') currencycode_dictid = 'CURRENCY_CODE'
    di.functioncode = 'return this.getx("' + currencycode_dictid + '").neosysxlate("CURRENCIES",1)'
    di.length = 30
}

function general_dict_exchangerate(di) {
    di.conversion = '[NUMBER,6Z]'
    di.length = 10
}

function general_pop_currencies(many, sselect) {
    if (!sselect)
        sselect = ''
    return neosysfilepopup('CURRENCIES', [['CURRENCY_NAME', 'Currency Name'], ['CURRENCY_CODE', 'Currency Code'], ['ISO_CODE', 'ISO Code'], ['CURRENT_RATE', 'Current Rate'], ['CURRENT_DATE', 'Current Date'], ['STOPPED', 'Stopped']], 1, sselect + ' BY STOPPED2 BY CURRENCY_NAME', many)
}

function formatcurrency(amount, currcode) {

    //amount can be an array of amounts
    //currcode may or may not be an array if amount is an array
    if (typeof amount == 'object') {
        var result = []
        var currcodeisarray = typeof currcode == 'object'
        for (ii = 0; ii < amount.length; ii++) {
            result[ii] = formatcurrency(amount[ii].toString(), currcodeisarray ? currcode[ii] : currcode)
        }
        return result
    }

    //determine the format
    var fmt
    if (currcode == gbasecurr)
        ndecs = 'BASE'
    else {
        var ndecs = currcode.neosysxlate('CURRENCIES', 3)
        if (ndecs == '') ndecs = 'BASE'
    }
    if (maxdecimalplaces) {
        if (ndecs == 'BASE') {
            if (gbasefmt)
                ndecs = Number(gbasefmt.substr(2, 1)) + 0
            else
                ndecs = 2
        }
        ndecs = Number(ndecs)
        var tt = amount.toString().split('.')[1]
        var inputndecs = tt ? tt.length : 0
        if (inputndecs >= ndecs)
            return amount
    }
    fmt = '[NUMBER,' + ndecs + ']'

    return neosysoconv(amount, fmt)

}

//not used anywhere yet ... see formatcurrency()
var gcurrencies = new Object
function getcurrency(currcode, propertyname) {

    var currencyobject = gcurrencies[currcode]
    if (!currencyobject) {
        currencyobject = new Object
        gcurrencies[currcode] = currencyobject
        //get the currency
        var currency = []
        if (!currency.neosysread('CURRENCIES', currcode)) {
            alert(currency.neosysresponse + '\nin getcurrency()')
            return 1
        }
        //record property
        currencyobject.record = currency
        //
        currencyobject.ndecimals = currency[3]
    }

    switch (propertyname) {
        case 'record':
            {
                return currencyobject.record
            }
        case 'ndecimals':
            {
                return currencyobject.ndecimals
            }
        default:
            {
                return ''
            }
    }

}

function getbaserate(currcode, date) {

    if (currcode == '') return ''

    //rate=1 for base curr
    if (currcode == gbasecurr) return 1

    //get the currency
    var currency = []
    if (!currency.neosysread('CURRENCIES', currcode)) {
        alert(currency.neosysresponse + '\nin getbaserate()')
        return 1
    }

    //locate rate for date
    var rate
    var dates = currency[4].split(vm)
    var rates = currency[5].split(vm)
    date = Number(date)//force a numerical comparison (cater for "012345" date format)
    for (var raten = 0; raten < dates.length; raten++) {
        if (dates[raten] <= date) break
    }
    if (raten >= rates.length) raten = rates.length - 1
    if (raten >= 0) {
        rate = rates[raten]
    }
    else {
        alert('There is no exchange rate on file for ' + neosysquote(currcode) + '\nUsing rate 1.0000')
        rate = 1
    }

    //default to next rate
    //if (!rate)
    //{
    // RATE=CURRENCY<5,VN-1>
    // if (RATE){
    //  MSG='NOTE: THE EXCHANGE RATE AS AT '+CURRENCY<4,VN-1> '[DATE,4*]'+' HAS BEEN USED BECAUSE'
    //  MSG+='|THERE IS NO RATE AS AT THE DATE OF THE VOUCHER - '+DATE '[DATE,4*]'
    //  GOSUB NOTE3
    // }
    //}

    //convert to numeric format
    rate = neosysnumber(rate)

    //check rate not zero
    if (rate < 0.00000000001) {
        alert('Exchange rate for ' + neosysquote(currcode) + ' is zero (0.0)!\n Using rate 1.0000')
        rate = 1
    }

    //return the rate
    return rate

}

function getexchrate(currcode, currcode2, date) {

    //rate=1 for same currency
    if (currcode == currcode2) return 1

    var rate = getbaserate(currcode, date)
    var rate2 = getbaserate(currcode2, date)

    //prevent infinity
    if (!rate2) return 1

    if (rate2 != 1) rate = neosysround(rate / rate2, 10)

    return rate

}


//users and security
////////////////////

function general_dict_usercode(di, many, withtask, haslocks, sselect) {
    if (!many)
        many = false
    if (many && !di.groupno)
        systemerror(di.name, '"Select Many" requires group>0')
    if (typeof withtask == 'undefined' || withtask == '')
        withtask = '' // '""'
    if (typeof haslocks == 'undefined' || haslocks == '')
        haslocks = '""'
    if (typeof sselect == 'undefined' || sselect == '')
        sselect = '""'
    if ("'\"".indexOf(sselect.substr(0, 1)) == -1)
        sselect = '"' + sselect.neosysswap('"', '\\"') + '"'
    di.popup = 'general_pop_users(' + many + ',"' + withtask + '",' + haslocks + ',' + sselect + ')'
    di.filename = 'USERS'
    di.validation = 'general_val_users()'
}

function general_pop_users(many, withtask, haslocks, sselect) {
    var sortselect = ''
    //currently available "authorisation groups" supported - easy to add any you like in DICT.USERS
    //AUTHORISED_JOURNAL_POST
    //AUTHORISED_TIMESHEET_ADMINISTRATION
    if (sselect)
        sortselect += ' AND ' + sselect
    if (withtask)
        sortselect += ' AND WITH AUTHORISED_' + withtask.neosysconvert(' ', '_').toUpperCase()
    //users tend not to have locks and departments/group tend to have locks
    if (typeof haslocks == 'boolean')
        sortselect += ' AND WITH KEYS ' + (haslocks ? 'NE' : 'EQ') + ' ""'
    sortselect = 'BY RANK ' + sortselect.slice(5)
    var selcol0 = 5
    return neosysfilepopup('USERS', [['USER_NAME', 'User Name'], ['DEPARTMENT_CODE', 'Department'], ['EMAIL_ADDRESS', 'Email'], ['LAST_LOGIN_DATETIME', 'Last Login Datetime'], ['LAST_LOGIN_LOCATION', 'Last Login Location'], ['USER_CODE', 'User Code']], selcol0, sortselect, many)
}

function general_val_users() {
    return true
}

function general_dict_username(di, usercodeid) {
    if (typeof usercodeid == 'undefined') usercodeid = 'USER_CODE'
    di.functioncode = 'return this.getx("' + usercodeid + '").neosysxlate("USERS",1)'
    di.length = 30
}

//functions
///////////

function general_pop_notimplemented() {
    alert('Sorry, not implemented yet')
    return null
}

function general_splitarray(array, mvgroups, n) {

    if (typeof array == 'string') array = array.split(fm)

    //pad empty fields
    if (typeof n != 'undefined') {
        for (var i = array.length; i < n; i++) if (typeof array[i] == 'undefined') array[i] = ''
    }

    for (var groupn = 0; groupn < mvgroups.length; groupn++) {

        var mvs = mvgroups[groupn]

        //split mv fields
        var maxlength = 1
        for (var i = 0; i < mvs.length; i++) {
            var mv1 = mvs[i]
            if (typeof mv1 == 'object') {
                mv2 = mv1[1]
                mv1 = mv1[0]
            }
            else mv2 = mv1
            for (mv = mv1; mv <= mv2; mv++) {
                if (typeof array[mv] != 'object') {
                    if (!array[mv])
                        array[mv] = ['']
                    else
                        array[mv] = array[mv].split(vm)
                    if (array[mv].length > maxlength)
                        maxlength = array[mv].length
                }
            }
        }

        //second pass to extend the length of all parallel multivalues
        // to the same number of values
        for (var i = 0; i < mvs.length; i++) {
            var mv1 = mvs[i]
            if (typeof mv1 == 'object') {
                mv2 = mv1[1]
                mv1 = mv1[0]
            }
            else mv2 = mv1
            for (mv = mv1; mv <= mv2; mv++) {
                while (array[mv].length < maxlength) array[mv][array[mv].length] = ''
            }
        }

    } //groupn

    return array

}

function general_dict_budgetcode(di, many, specific) {
    di.conversion = general_budgetcodes(many, specific)
    di.validation = 'general_val_budgetcode()'
}

function general_val_budgetcode() {

    if (gvalue != 'SELECTED') return true

    cols = [[1, 'Option'], [0, 'Code']]
    var categorycodes = neosysdecide2('', general_budgetcodes(), cols, 0, '', many = true)

    if (!categorycodes) return false

    gvalue = categorycodes.join(',')

    return true

}

function general_budgetcodes(many, specific) {
    var t = ''
    if (!specific) {
        t += ':ACTUAL;Actual'
        t += ':BUDGET;Budget'
        t += ':FORECAST;Forecast'
        if (many) t += ':SELECTED;<Select Many>'
        t += ':BUDGETFORECAST;Budget/Forecast'
    }
    for (var i = 1; i <= nbudgets; i++) t += ':BUDGET ' + i + ';Budget ' + i
    for (var i = 1; i <= nbudgets; i++) t += ':FORECAST ' + i + ';Forecast ' + i

    return t.substr(1)

}

function general_open_version() {
    var version = gds.getx('VERSION')
    //if (!version) version='1'
    if (!version) return neosysinvalid('No previous version')
    if (gvalue == version)
        return neosysinvalid('You are looking at version ' + gvalue + ' already')

    var key = gkey.neosysfield('~', 1) + '~' + gvalue
    //windowopen(window.location.href.toString().split('?')[0],{"key":key})
    windowopen(window.location.href, { "key": key })
    return true
}

//if changed here also change manual version in schedules_dict
function general_dict_addlogfields(dict, groupn, logfn, linkfunction, prefix, postfix) {

    if (!prefix) prefix = ''
    if (!postfix) postfix = '_UPDATED'

    var din = dict.length - 1

    di = dict[++din] = dictrec(prefix + 'USERNAME' + postfix, 'F', logfn + 0, '', groupn)
    di.copyable = false
    di.noinsertrow = true
    di.nodeleterow = true
    di.readonly = true

    di = dict[++din] = dictrec(prefix + 'DATETIME' + postfix, 'F', logfn + 1, '', groupn)
    neosys_dict_datetime(di)
    di.align = 'left'
    di.copyable = false
    di.noinsertrow = true
    di.nodeleterow = true
    di.readonly = true

    di = dict[++din] = dictrec(prefix + 'STATION' + postfix, 'F', logfn + 2, '', groupn)
    di.copyable = false
    di.noinsertrow = true
    di.nodeleterow = true
    di.readonly = true

    di = dict[++din] = dictrec(prefix + 'VERSION' + postfix, 'F', logfn + 3, '', groupn)
    di.copyable = false
    di.noinsertrow = true
    di.nodeleterow = true
    if (linkfunction)
        di.link = linkfunction
    di.readonly = true
    di.lowercase = true//version is sometimes filenames which can be lowercase

    di = dict[++din] = dictrec(prefix + 'STATUS' + postfix, 'F', logfn + 4, '', groupn)
    di.copyable = false
    di.noinsertrow = true
    di.nodeleterow = true
    di.readonly = true
    di.lowercase = true

    return

}

function general_open_archive(prefix, postfix) {

    if (!prefix) prefix = ''
    if (!postfix) postfix = '_ARCHIVED'

    var archiveno = gds.getx('VERSION' + postfix, grecn)[0]
    if (!archiveno) return neosysinvalid('Sorry, nothing on this line')

    params = {}
    params.database = gdataset
    params.filename = gdatafilename
    params.key = gkey
    params.versionno = archiveno
    params.allowupdate = false
    params.allowdelete = false

    neosysshowmodaldialog('../NEOSYS/upload.htm', params)

    return true

}

function general_add_archive() {

    if (!glocked)
         return readonlydocmsg()

        //ensure that we can save immediately after uploading file
        if (gchangesmade && !savedoc())
            return false

    //}

    //var
    //gnewstatus to be global to carry forward into general_postupload()
    gnewstatus = neosysinput('Please enter a comment or description about the file being uploaded.')
    if (!gnewstatus)
        return false

    //get last archive number. allow for last line to be blank
    //var
    //garchivenos to be global to carry forward into general_postupload()
    garchivenos = gds.getx('VERSION_ARCHIVED', null)
    var lastarchiveno = garchivenos.join(',').neosystrim(',').split(',')
    lastarchiveno = lastarchiveno[lastarchiveno.length - 1]
    //split off initial version number
    lastarchiveno = lastarchiveno.neosysfield('.', 1)
    if (!lastarchiveno)
        lastarchiveno = 0

    var newarchiveno = Number(lastarchiveno) + 1

    params = {}
    params.database = gdataset
    params.filename = gdatafilename
    params.key = gkey
    params.versionno = newarchiveno
    params.newstatus = gnewstatus
    params.ensurenotlocked = glocked ? 0 : 1

    var targetfilename = neosysshowmodaldialog('../NEOSYS/upload.htm', params)
    //it appears that internet explorer will return here with the targetfilename after calling postload in upload.htm
    //whereas other browsers will call here general_postupload() and return null for targetfilename
    if (!targetfilename)
        return false
    //windowopen('../NEOSYS/upload.htm',params)

    general_postupload(targetfilename)

    return true

}

function general_postupload(targetfilename) {

    if (!glocked) {
        db.request = 'EXECUTE\rGENERAL\r\POSTUPLOAD'
        //+gparameters.originalkeyversionno
  + '\r'
  + '\r' + gdatafilename
  + '\r' + gkey
  + '\r' + targetfilename.neosysfield('\\', -1)
  + '\r' + gnewstatus

        if (!db.send())
            return neosysinvalid(db.response)

        //return true
    }

    //get/add rown
    var rown = garchivenos.length - 1
    if (garchivenos[rown] != '') {
        gds.addrow(gds.dictitem('VERSION_ARCHIVED').groupno)
        ++rown
    }

    //gds.setx('VERSION_ARCHIVED',rown,newarchiveno)
    gds.setx('VERSION_ARCHIVED', rown, targetfilename.neosysfield('\\', -1))
    gds.setx('STATUS_ARCHIVED', rown, gnewstatus)
    /* will be added on server when doc is saved in next section in JOB.SUBS VOUCHER.SUBS/PREWRITE
    // lack of username indicates to server side PREWRITE NOT to add version log
    gds.setx('USERNAME_ARCHIVED',rown,)
    gds.setx('DATETIME_ARCHIVED',rown,)
    gds.setx('STATION_ARCHIVED',rown,)
    */

    if (glocked)
        writedoc()

    return true

}
