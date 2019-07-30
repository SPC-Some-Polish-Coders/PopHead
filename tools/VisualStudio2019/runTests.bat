call "%VSConsole%"
devenv ..\..\PopHead.sln /Build Tests /Project Tests /ProjectConfig Tests
start /D ..\..\ /B ..\..\TestsProj\bin\bin\Tests.exe
PAUSE
