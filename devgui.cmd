call config DEV > config.log
@echo BOOST32=%BOOST32%
@echo BOOST64=%BOOST64%
@echo POSTGRES32=%POSTGRES32%
@echo POSTGRES64=%POSTGRES64%
@echo ICU32=%ICU32%
@echo ICU64=%ICU64%
%EXO_DEVCMD% %EXO_DEVOPT%
if "%EXO_BATCHMODE%" == "" pause
