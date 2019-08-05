call clearFiles.bat
cd ..\..\vendor\premake
premake5 --file=..\..\tools\premake\premake5.lua vs2019
cd ..\VisualStudio2019
mkdir ..\..\logs
PAUSE
