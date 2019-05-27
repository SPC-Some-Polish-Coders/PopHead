workspace "PopHead"
    architecture "x86"
    location "../"
    
    configurations{
        "Debug",
        "Release"
    }

project "PopHead"
    location "../"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++14"

    targetdir ("../bin/bin/")
	objdir ("../bin/obj/")
    
    includedirs{
        "../src",
        "../vendor/SFML_2.5.1-VisualStudio/include"
    }

    libdirs{
        "../vendor/SFML_2.5.1-VisualStudio/lib"
    }

    files{
        "../src/**.hpp",
        "../src/**.cpp",
        "../src/**.inl"
    }

    links{
        "opengl32.lib",
        "winmm.lib",
        "gdi32.lib",
        "freetype.lib"
    }

    filter "configurations:Debug"
        symbols "On"

        defines{
            "DEBUG",
            "SFML_STATIC"
        }

        links{
            "sfml-graphics-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d"
        }

    filter "configurations:Release"
        optimize "On"

        defines{
            "RELEASE",
            "SFML_STATIC"
        }

        links{
            "sfml-graphics-s",
            "sfml-audio-s", 
            "sfml-network-s",
            "sfml-window-s",
            "sfml-system-s"
        }
