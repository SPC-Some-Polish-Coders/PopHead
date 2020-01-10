copy ..\..\vendor\SFML_2.5.1\dll\openal32.dll ..\..\
cd ..\..\vendor\premake
premake5 --file=..\..\tools\premake\premake5.lua vs2019
cd ..\..\
mkdir logs
