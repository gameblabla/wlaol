set path=path+;h:\cygwin\bin;h:\cygwin\usr\local\mingw32\bin

make -f Makefile.gp2x clean
make -f Makefile.gp2x


@echo off
if NOT ERRORLEVEL 1 GOTO RUN
pause
goto END

:RUN
copy act.gpe ..\act.gpe
make -f Makefile.gp2x clean
goto END

:END
