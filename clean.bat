:checkconfig
@if exist config.bat goto gotconfig
@echo Please first copy config0.bat to config.bat and 
@echo edit the your new config.bat to your configuration
pause
exit
:gotconfig

call config.bat

@echo %EXODUS_CLEAN%
%EXODUS_CLEAN%

if "%EXODUS_BATCHMODE%" == "" pause