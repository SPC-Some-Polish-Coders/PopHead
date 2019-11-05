root_dir = "../../"
config_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
exe_dir = root_dir .. "bin/" .. config_dir .. "/%{prj.name}"
obj_dir = root_dir .. "bin-obj/" .. config_dir .. "/%{prj.name}"

workspace "PopHead"
    architecture "x86"
    location (root_dir)
    startproject "PopHead"
    
    configurations{
        "Debug",
        "Release",
        "Distribution",
        "Tests"
    }

project "PopHead"
    location (root_dir)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    
    targetdir (exe_dir)
	objdir (obj_dir)
    
    debugdir "%{wks.location}"
    
    includedirs{
        root_dir .. "src",
        root_dir .. "vendor/SFML_2.5.1/include",
		root_dir .. "vendor/glew-1.5.4/include",
		root_dir .. "vendor/stb",
        root_dir .. "vendor/entt-3.2.0/src"
    }

    libdirs{
		root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio",
		root_dir .. "vendor/glew-1.5.4/lib"
	}

    files{
        root_dir .. "src/**.hpp",
        root_dir .. "src/**.cpp",
        root_dir .. "src/**.inl"
    }

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
        "openal32.lib",
		"glew32s.lib"
    }

	ignoredefaultlibraries { "libcmt" }

    defines{
		"SFML_STATIC",
		"GLEW_STATIC"
	}

    filter "configurations:Debug"
        symbols "On"

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

    filter{"configurations:Distribution"}
        defines{"PH_DISTRIBUTION"}

    filter "system:Windows"
        defines{"PH_WINDOWS"}

    filter "system:Unix"
        defines{"PH_LINUX"}

    filter "system:Mac"
        defines{"PH_MAC"}

    filter{}

project "Tests"
    location (root_dir)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir (exe_dir)
	objdir (obj_dir)
    
    debugdir "%{wks.location}"
    
    includedirs{
        root_dir .. "src",
        root_dir .. "vendor/SFML_2.5.1/include",
        root_dir .. "vendor/entt-3.2.0/src",
        root_dir .. "vendor/catch2"
    }

    libdirs{root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio"}

    files{
        root_dir .. "src/**.hpp",
        root_dir .. "src/**.cpp",
        root_dir .. "src/**.inl",
        root_dir .. "tests/**.hpp",
        root_dir .. "tests/**.cpp",
        root_dir .. "tests/**.inl"
    }
    
    removefiles{
        root_dir .. "src/main.cpp"
    }

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

    defines{"SFML_STATIC"}

    filter "configurations:Debug or Tests"
        symbols "On"

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
        
    filter{"configurations:Tests"}
        defines{"PH_TESTS"}
        
    filter "system:Windows"
        defines{"PH_WINDOWS"}

    filter "system:Unix"
        defines{"PH_LINUX"}

    filter "system:Mac"
        defines{"PH_MAC"}

    filter{}
    
printf("For now PopHead supports only new Visual Studio versions and Codeblocks.")
printf("If you have any problems with Premake or compiling PopHead contact Grzegorz \"Czapa\" Bednorz.")