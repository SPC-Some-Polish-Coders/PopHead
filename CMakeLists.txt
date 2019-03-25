cmake_minimum_required (VERSION 3.13)

project( PopHead CXX )

set( SRC_LIST
src/main.cpp
src/Base/game.cpp
src/Resources/fontResource.cpp
src/Resources/textureResource.cpp
src/Resources/shaderResource.cpp
src/Resources/soundBufferResource.cpp
src/Renderer/renderer.cpp
src/Renderer/layer.cpp
src/States/stateMachine.cpp
src/States/state.cpp
src/States/GreenState/greenState.cpp
src/States/OrangeState/orangeState.cpp
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
