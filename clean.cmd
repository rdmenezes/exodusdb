call config CLEAN > config.log
@echo EXO_CLEAN_CMD=%EXO_CLEANCMD%
@echo EXO_CLEAN_OPT=%EXO_CLEANOPT%
%EXO_CLEANCMD% %EXO_CLEANOPT%
if "%EXO_BATCHMODE%" == "" pause
