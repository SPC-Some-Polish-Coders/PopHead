call "%VSConsole%"
devenv ..\..\PopHead.sln /Build Tests /Project Tests /ProjectConfig Tests
start /D ..\..\ /B ..\..\bin\Tests-windows-x86\Tests\Tests.exe
PAUSE
