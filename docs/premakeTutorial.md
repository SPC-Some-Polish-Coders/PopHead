# Premake usage tutorial
-------------------------------------------------------

## What is Premake
Premake is an project build system. It can generate project files for several IDEs. Premake scripts are written in Lua.

## How to use Premake to generate PopHead project files
We can use premake only from command line. There is no GUI for it. But don't worry. I assure you that it is very convinient and easy.
You have to open command line in the premake directory. If you're on Windows you can do that clicking to bar with link to the current
directory in your windows explorer and type cmd. This will open command prompt in your current directory. <br/> 
If you enter `premake5 --help` you should see help for premake5 and available actions. <br/>
The same thing for premake 4: `premake4 --help`. <br/>
You can generate project files for any IDE which is supported, but truly we support only Visual Studio and CodeBlocks.
That is because in vendor directory we only have .lib files for Visual Studio and CodeBlocks.

### Visual Studio
To generate project files for VisualStudio enter: <br/>
`premake5 vs2019` for Visual Studio 2019 <br/>
or <br/>
`premake5 vs2017` for Visual Studio 2017 <br/>

### CodeBlocks
To generate project files for CodeBlocks enter: <br/>
`premake4 codeblocks` <br/>

## Clean action
Clean action deletes project files for every IDE. <br/>
Open command prompt in premake directory and type in: <br/>
`premake4 clean`

## What if I want to code on other IDE then Visual Studio and CodeBlocks
In that case contact Grzegorz "Czapa" Bednorz. Probably we'll make support for this IDE as well. <br/>
Premake works fine to every IDE. External libaries in vendor directory make make the problem. <br/>
If you're not part of the PopHead team and you want to build it on IDE which we don't support just make your own configuration.

## What if I want to run premake on Linux or Mac
For now we only support windows (We have windows premake binaries) because every PopHead developer works on Windows. <br/>
But you can just download premake for another OS from here: https://premake.github.io/download.html#v4 and place the binaries
in premake directory.

## Premake5 and Premake4
PopHead supports Premake5 and Premake4. Premake5 is newer and has several usefull features which Premake4 doesn't have.
We support Premake4 because it can generate project files for some IDEs which are not supported in Premake5 for example CodeBlocks.
Premake4 also has very usefull clean action which dosen't work in Premake5.

## Issues
If you have any issues with Premake just write the Github issue or contact us.
