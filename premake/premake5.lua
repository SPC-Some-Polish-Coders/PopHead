workspace "PopHead"
    location "../"
    configurations{"Debug", "Release"}
    architecture "x86"

project "PopHead"
    location "../"
    kind "ConsoleApp"
    language "C++"
    targetdir ("../bin/bin/")
	objdir ("../bin/obj/")
    includedirs{"../src", "../vendor/SFML_2.5.1-VisualStudio/include"}
    libdirs{"../vendor/SFML_2.5.1-VisualStudio/lib"}

    files{
        "../src/**.hpp",
        "../src/**.cpp",
        "../src/**.inl"
    }

    links{"opengl32.lib"}
    links{"winmm.lib"}
    links{"gdi32.lib"}
    links{"freetype.lib"}

    filter "configurations:Debug"
        defines{"DEBUG", "SFML_STATIC"}
        symbols "On"
        links { "sfml-graphics-s-d" }
        links { "sfml-audio-s-d" }
        links { "sfml-network-s-d" }
        links { "sfml-window-s-d" }
        links { "sfml-system-s-d" }

    filter "configurations:Release"
        defines{"RELEASE", "SFML_STATIC"}
        optimize "On"
        links { "sfml-graphics-s" }
        links { "sfml-audio-s" }
        links { "sfml-network-s" }
        links { "sfml-window-s" }
        links { "sfml-system-s" }
