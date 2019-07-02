call %VSConsole%"
devenv ..\PopHead.sln /Build Tests /Project Tests /ProjectConfig Tests
..\TestsProj\bin\bin\Tests.exe
PAUSE
