workspace "PopHead"
    architecture "x86"
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
    cppdialect "C++17"

    targetdir ("../bin/bin/")
	objdir ("../bin/obj/")
    
    includedirs{
        "../src",
        "../vendor/SFML_2.5.1-VisualStudio/include"
    }

    libdirs{"../vendor/SFML_2.5.1-VisualStudio/lib"}

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

    defines{"SFML_STATIC"}

    filter "configurations:Debug"
        symbols "On"

        defines{"PH_DEBUG"}

        links{
            "sfml-graphics-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d"
        }

    filter{"configurations:Release or Distribution"}
        optimize "On"

        links{
            "sfml-graphics-s",
            "sfml-audio-s", 
            "sfml-network-s",
            "sfml-window-s",
            "sfml-system-s"
        }

    filter{"configurations:Release"}
        defines{"PH_RELEASE"}

    filter{"configurations:Distribution"}
        defines{"PH_DISTRIBUTION"}