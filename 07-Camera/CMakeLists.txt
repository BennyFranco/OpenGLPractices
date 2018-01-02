cmake_minimum_required (VERSION 2.6)
project (07-Camera)

set(SOURCES Camera.cpp Camera.h Shader.cpp Shader.h Main.cpp)

include_directories("../include")
add_library(glew32 STATIC IMPORTED)
add_library(glfw3 STATIC IMPORTED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(07-Camera ${SOURCES})


if(WIN32)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "..\\..\\lib\\Win32\\glew32s.lib")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "..\\..\\lib\\Win32\\glfw3.lib")

    target_link_libraries(07-Camera ${OPENGL_LIBRARIES} glew32 glfw3)
    
    set(BUILD_DIR "./Debug")

elseif(APPLE)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libGLEW.a")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libglfw3.a")

    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
    target_link_libraries(07-Camera glew32 glfw3)
    set(BUILD_DIR ".")
endif()


file(GLOB IMAGES "./images/*.*")
file(GLOB SHADERS "*.vs" "*.fs")
file(COPY ${IMAGES} DESTINATION ${BUILD_DIR}/images)
file(COPY ${SHADERS} DESTINATION ${BUILD_DIR}/)