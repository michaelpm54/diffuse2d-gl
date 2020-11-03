cmake_minimum_required(VERSION 3.14)
project(GameboyVramViewer LANGUAGES CXX)

set(EXE diffuse2d)

if (UNIX)
	option(FETCH_LIBS "Download dependencies automatically" OFF)
else()
	option(FETCH_LIBS "Download dependencies automatically" ON)
endif()

if (FETCH_LIBS)
	include(cmake/depends.cmake)
endif()

add_executable(${EXE}
	src/engine.cpp
	src/engine.h
	src/font_manager.h
	src/game_object.cpp
	src/game_object.h
	src/gfx_device.cpp
	src/gfx_device.h
	src/gl_types.h
	src/gl_util.cpp
	src/gl_util.h
	src/main.cpp
	src/render_component.cpp
	src/render_component.h
	src/renderer.cpp
	src/renderer.h
	src/singleton.h
	src/tilemap.cpp
	src/tilemap.h
	src/timer.cpp
	src/timer.h
	src/transform_component.cpp
	src/transform_component.h
	src/window.h
	src/window_sdl.cpp
	src/window_sdl.h
	src/world.cpp
	src/world.h
	src/app_gl.h
	lib/glew.cpp
)

if (UNIX)
	find_package(glm CONFIG REQUIRED)
	find_package(SDL2 CONFIG REQUIRED)
	target_link_libraries(${EXE} dl glm SDL2 GL)
else ()
	target_link_libraries(${EXE} glm SDL2 opengl32)
endif()

target_include_directories(${EXE} PRIVATE include)

set_property(TARGET ${EXE} PROPERTY CXX_STANDARD 20)

target_include_directories(${EXE}
	PRIVATE
		${FETCHCONTENT_BASE_DIR}/sdl2-src/include
)

add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXE}>/assets
    DEPENDS ${EXE})
