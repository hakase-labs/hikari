
rem echo Copying game content to output directory.
rem xcopy /S %1\*.* %2\ /y

cd %3\tools
rem echo Generating documentation...
rem doxygen.exe default.doxygen

echo Packaging game content...
echo %2
7za.exe a %2\content.zip %1\* -xr!.svn -xr!*.dll -tzip
rem xcopy /C /R /Y %1\*.nut %2\
xcopy /C /R /Y %1\*.dll %2\
cd ..