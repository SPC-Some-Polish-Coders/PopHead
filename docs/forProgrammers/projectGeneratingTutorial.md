# Premake usage tutorial
-------------------------------------------------------

## Table of Contents
<details>
<summary>"Click to expand"</summary>
  
- [What is Premake](#what-is-premake)
- [How to use Premake to generate PopHead project files?](#how-to-use-premake-to-generate-pophead-project-files)
- [Project's scripts](#projects-scripts)
  - [Batch scripts](#batch-scripts)
- [What if I want to code on other IDE than Visual Studio?](#what-if-i-want-to-code-on-other-ide-than-visual-studio)
- [What if I want to run premake on Linux or Mac?](#what-if-i-want-to-run-premake-on-linux-or-mac)
- [Premake5 and Premake4](#premake5-and-premake4)
- [Issues](#issues)

</details>

## What is Premake
Premake is a project build system. It can generate project files for several IDEs. Premake scripts are written in Lua.
If you want to see premake's website click [here](https://premake.github.io/).

## How to use Premake to generate PopHead project files
We can use premake only from command line. There is no GUI for it. But don't worry. We assure you that it is very convinient and easy.
You can enter `premake5 --help` in vendor/premake directory to see available actions.
You can generate project files for any IDE which is supported by premake, but for now we care only for Visual Studio 2019.
Our script to generate the project in in tools/premake directory, but you can use batch scipt [Scripts](#batch-scripts)
That is because all present programmers in team work on VS2019 so we tested only that IDE.

## Project's scripts
In 'tools/' directory we have a bunch of useful scripts. They are segregated by operation systems and programs. We made scripts only for Windows and VS, because that's the configuration of all PopHead programmers for now. If you want to add additional scripts, feel free to do it!

#### Batch scripts
- Windows
  - logsCleaner.bat - clears all logs files in logs/ directory
- VisualStudio 2019
  - configureProject.bat - runs premake script and generates project files for VS2019, copies one dll file (openal32.dll)
  - clearProjectFiles.bat - removes VS files (including executables) in all project directories
  - runTest.bat - convenient tool for running unit tests in project without running VS. To make it work you need to create environment variable in Windows called 'VSConsole' with the path to VS Console tool (example: C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat). If you have a custom directory of VS you need to find it on your own

## What if I want to code on other IDE than Visual Studio
In that case contact us (Piotr Gardocki or Grzegorz "Czapa" Bednorz). Probably we'll make support for this IDE as well. Premake works fine for every IDE.
If you're not a part of the PopHead team and you want to build the project on IDE which we don't support just make your own configuration.

## What if I want to run premake on Linux or Mac
For now we only support windows (We have windows premake binaries) because every PopHead developer works on Windows. <br/>
But you can just download premake for another OS from [here](https://premake.github.io/download.html#v4) and place the binaries in premake directory.

## Premake5 and Premake4
In PopHead we have both Premake4 and Premake5 executables. But we have only script for Premake5. Premake5 is the newest version, but still doesn't support some IDEs like CodeBlocks. If you wonder if your IDE is supported check out official [premake tutorial](https://github.com/premake/premake-core/wiki/Using-Premake), or contact us.

## Issues
If you have any issues with Premake just write the Github issue or contact us.
