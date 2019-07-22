call clearFiles.bat
cd ..\premake
premake5 vs2019
cd ..\VisualStudio2019
call ..\Windows\getModulesNames.bat
cd ../../
mkdir logs
PAUSE
