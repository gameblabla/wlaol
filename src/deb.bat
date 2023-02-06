set path=path+;h:\cygwin\bin;h:\cygwin\usr\local\mingw32\bin

windres -i bash.rc -o bush.o
make -f Makefile.win
make -f Makefile.win

@echo off
if NOT ERRORLEVEL 1 GOTO RUN
pause
goto END

:RUN
copy act.exe ..\act.exe
cd ..\
act.exe
cd src
REM make -f Makefile.win clean
goto END

:END

