solution "PopHead"
    location "../"

    configurations{
        "Debug",
        "Release",
        "Distribution"
    }

project "PopHead"
    location "../"
    kind "ConsoleApp"
    language "C++"

    targetdir "../bin/bin/"
    objdir "../bin/obj/"

    includedirs{
        "../src",
        "../vendor/SFML_2.5.1-CodeBlocks/include"
    }

    libdirs{"../vendor/SFML_2.5.1-CodeBlocks/lib"}

    files {
        "../src/**.hpp",
        "../src/**.cpp",
        "../src/**.inl",
    }
    
    defines{"SFML_STATIC"}
    
    configuration "Debug"
        flags{"Symbols"}
        
        links{
            "sfml-graphics-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d"
        }
    
    configuration "Release or Distribution"
        flags{"Optimize"}
        
        links{
            "sfml-graphics-s",
            "sfml-audio-s", 
            "sfml-network-s",
            "sfml-window-s",
            "sfml-system-s"
        }
    
    configuration "Distribution"
        defines{"PH_DISTRIBUTION"}
    
    configuration{}
    
    links{
        "opengl32.lib",
        "winmm.lib",
        "gdi32.lib",
        "freetype.lib",
        "flac.lib",
        "vorbisenc.lib",
        "vorbisfile.lib",
        "vorbis.lib",
        "ogg.lib",
        "openal32.lib"
    }

    configuration "windows"
        defines{"PH_WINDOWS"}

    configuration "linux"
        defines{"PH_LINUX"}

    configuration "mac"
        defines{"PH_MAC"}

    printf("For now PopHead supports only new Visual Studio versions and Codeblocks.")
    printf("If you have any problems with Premake or compiling PopHead contact Grzegorz \"Czapa\" Bednorz.")