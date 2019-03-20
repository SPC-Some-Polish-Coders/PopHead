cmake_minimum_required (VERSION 3.13)

project( PopHead CXX )

set( SRC_LIST
src/main.cpp
src/Base/Game.cpp
src/Resources/FontResource.cpp
src/Resources/TextureResource.cpp
src/Resources/ShaderResource.cpp
src/Resources/SoundBufferResource.cpp
src/Renderer/Renderer.cpp
src/States/StateMachine.cpp
src/States/State.cpp
)

include_directories(
"${PROJECT_BINARY_DIR}"
"${CMAKE_SOURCE_DIR}"
"${CMAKE_SOURCE_DIR}/src"
"${CMAKE_SOURCE_DIR}/ref/include"
)

set( COMMON_LIBS
	-lasan
    -lubsan
	-lfreetype
	-lGL
	-ludev
	-logg
	-lvorbis
	-lvorbisenc
	-lvorbisfile
	-lopenal
	-lpthread
	sfml-graphics-s.a
	sfml-window-s.a
	sfml-audio-s.a
	sfml-network-s.a
	sfml-system-s.a
)

set( LINUX_LIBS
	-lX11
	-lXrandr
)

set( WINDOWS_LIBS
	sfml-main-s.a
)

if(WIN32)
	set(SYSTEM_DEPENDENT_LIBS ${WINDOWS_LIBS})
endif(WIN32)
if(UNIX)
	set(SYSTEM_DEPENDENT_LIBS ${LINUX_LIBS})
endif(UNIX)

add_executable( PopHead ${SRC_LIST} )
set_property(TARGET PopHead PROPERTY CXX_STANDARD 17 )

target_include_directories(PopHead PUBLIC
	src
	res/include)

target_link_directories(PopHead PUBLIC res/lib/SFML )

target_link_libraries( PopHead PUBLIC
		${COMMON_LIBS}
		${SYSTEM_DEPENDENT_LIBS}
	 )

target_compile_options(PopHead PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                                -Wall -Wextra -fsanitize=address -fsanitize=undefined>
                           $<$<CXX_COMPILER_ID:MSVC>:
                                W4>)
