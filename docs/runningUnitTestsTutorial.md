# Running unit tests tutorial
-----------------------------
## Catch2
We're using Catch2 framework for writing unit tests. If you don't know Catch or you don't know how to write unit tests at all,
I recommend you to check out this tutorial: https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md.
Later you should also read the documentation: https://github.com/catchorg/Catch2/blob/master/docs/Readme.md#.

## How to set up and run unit tests in PopHead
For now the tests are set up only for the Visual Studio, because currently every PopHead developer is working on Visual Studio.
If you are a new person in the PopHead team and you're not using Visual Studio, please contact Piotr Gardocki known also as the peterekg999.
If you're not a part of the PopHead team and you want to run unit tests and you don't have Visual Studio I'm afraid you have to handle it yourself.
### Setting the system environment variable
If you are working on Windows you should first set up the system environment variable. If you don't know what the environment variable is or
how to set it up. You can check out this tutorial: https://www.twilio.com/blog/2017/01/how-to-set-environment-variables.html. <br>
The new variable has to have name ``VSConsole`` and the value should be the path to your VsDevCmd.bat file.
I had to set value to this: ``C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat``.
On your computer the path can be slightly different.
### Compiling and running tests
Now you can eventually run your tests by double clicking on ``runTests.bat`` which is in ``PopHead/tools/VisualStudio2019`` directory.
The batch file will compile and run your tests.
