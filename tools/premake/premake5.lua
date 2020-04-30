root_dir = "../../"
config_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
exe_dir = root_dir .. "bin/" .. config_dir .. "/%{prj.name}"
obj_dir = root_dir .. "bin-obj/" .. config_dir .. "/%{prj.name}"

workspace "PopHead"
    location (root_dir)
    startproject "PopHead"

    configurations{
        "Debug_32bit",
        "Release_32bit",
        "Distribution_32bit",
        "Profiling_32bit",
        "Tests_32bit",
        "Debug_64bit",
        "Release_64bit",
        "Distribution_64bit",
        "Profiling_64bit",
        "Tests_64bit"
    }

	filter "configurations:*32bit"
		architecture "x86"

	filter "configurations:*64bit"	
		architecture "x64"

	filter {}

project "PopHead"
    location (root_dir)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    
    targetdir (exe_dir)
	objdir (obj_dir)

	pchheader "pch.hpp"
	pchsource "../../src/pch.cpp"
    
    debugdir "%{wks.location}"

    includedirs{
        root_dir .. "src",
        root_dir .. "vendor/SFML_2.5.1/include",
		root_dir .. "vendor/glew-2.1.0/include",
		root_dir .. "vendor/stb",
        root_dir .. "vendor/entt-3.2.0/src",
        root_dir .. "vendor/imgui"
    }

	filter "configurations:*32bit"
		libdirs{
			root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio-32bit",
			root_dir .. "vendor/glew-2.1.0/lib/32bit"
		}
	filter "configurations:*64bit"
		libdirs{
			root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio-64bit",
			root_dir .. "vendor/glew-2.1.0/lib/64bit"
		}
	filter{}

    files{
        root_dir .. "src/**.hpp",
        root_dir .. "src/**.cpp",
        root_dir .. "src/**.inl",
		root_dir .. "vendor/imgui/**.h",
		root_dir .. "vendor/imgui/**.cpp"
    }

	removefiles{
		root_dir .. "src/dearImGui.cpp"
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

	ignoredefaultlibraries "libcmt"
	disablewarnings { "4996" } -- disable depricated C std libary functions 

    defines{
		"SFML_STATIC",
		"GLEW_STATIC"
	}
	
    filter "configurations:Debug*"
        symbols "On"

        links{
            "sfml-graphics-s-d",
            "sfml-audio-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d"
        }

	filter {"configurations:Debug*", "action:vs*"}
		inlining "Explicit"

    filter "configurations:Release* or Distribution* or Profiling*"
        optimize "On"

        links{
            "sfml-graphics-s",
            "sfml-audio-s", 
            "sfml-window-s",
            "sfml-system-s",
            "sfml-main"
        }

    filter "configurations:Distribution* or Profiling*"
        kind "WindowedApp"

	filter "configurations:Distribution*"
        defines "PH_DISTRIBUTION" 

	filter "configurations:Profiling*"
		defines "PH_PROFILING"

    filter "system:Windows"
        defines "PH_WINDOWS"

    filter "system:Unix"
        defines "PH_LINUX"

    filter "system:Mac"
        defines "PH_MAC"

    filter{}



project "Tests"
    location (root_dir)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    
    targetdir (exe_dir)	
    objdir (obj_dir)

	pchheader "pch.hpp"
	pchsource "../../src/pch.cpp"
    
    debugdir "%{wks.location}"
    
    includedirs{
        root_dir .. "src",
        root_dir .. "vendor/SFML_2.5.1/include",
        root_dir .. "vendor/glew-2.1.0/include",
        root_dir .. "vendor/stb",
        root_dir .. "vendor/entt-3.2.0/src",
        root_dir .. "vendor/catch2"
    }
   
	filter "configurations:*32bit"
		libdirs{
			root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio-32bit",
			root_dir .. "vendor/glew-2.1.0/lib/32bit"
		}
	filter "configurations:*64bit"
		libdirs{
			root_dir .. "vendor/SFML_2.5.1/lib-VisualStudio-64bit",
			root_dir .. "vendor/glew-2.1.0/lib/64bit"
		}
	filter{}

    files{
        root_dir .. "src/**.hpp",
        root_dir .. "src/**.cpp",
        root_dir .. "src/**.inl",
        root_dir .. "tests/unitTests/**.hpp",
        root_dir .. "tests/unitTests/**.cpp",
        root_dir .. "tests/unitTests/**.inl"
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
        "openal32.lib",
        "glew32s.lib"
    }

	ignoredefaultlibraries { "libcmt" }
	disablewarnings { "4996" } -- disable depricated C std libary functions 

    defines{
		"SFML_STATIC",
		"GLEW_STATIC"
	}
	
    filter "configurations:Debug* or Tests*"
        symbols "On"

        links{
            "sfml-graphics-s-d",
            "sfml-audio-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d"
        }

	filter {"configurations:Debug*", "action:vs*"}
		inlining "Explicit"

    filter "configurations:Release* or Distribution* or Profiling*"
        optimize "On"

        links{
            "sfml-graphics-s",
            "sfml-audio-s", 
            "sfml-window-s",
            "sfml-system-s",
        }

	filter "configurations:Tests*"
		defines "PH_TESTS"

    filter "configurations:Distribution* or Profiling*"
        kind "WindowedApp"

	filter "configurations:Distribution*"
        defines "PH_DISTRIBUTION"

	filter "configurations:Profiling*"
		defines "PH_PROFILING"

    filter "system:Windows"
        defines "PH_WINDOWS" 

    filter "system:Unix"
        defines "PH_LINUX"

    filter "system:Mac"
        defines "PH_MAC"

    filter{}
    
printf("For now PopHead supports only new Visual Studio versions.")
printf("If you have any problems with Premake or compiling PopHead contact Grzegorz \"Czapa\" Bednorz.")
