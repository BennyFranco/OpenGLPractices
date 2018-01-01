cmake_minimum_required (VERSION 2.6)
project (08-Colors)

set(SOURCES Camera.cpp Camera.h Shader.cpp Shader.h Main.cpp)

include_directories("../include")
add_library(glew32 STATIC IMPORTED)
add_library(glfw3 STATIC IMPORTED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})


if(WIN32)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "..\\lib\\Win32\\glew32s.lib")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "..\\lib\\Win32\\glfw3.lib")
elseif(APPLE)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libGLEW.a")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libglfw3.a")

    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()

add_executable(08-Colors ${SOURCES})
target_link_libraries(08-Colors glew32 glfw3)

file(GLOB IMAGES "./images/*.*")
file(GLOB SHADERS "*.vs" "*.fs")
file(COPY ${IMAGES} DESTINATION ./images)
file(COPY ${SHADERS} DESTINATION ./)