cmake_minimum_required (VERSION 2.6)
project (17.1-Blending)

set(HEADERS Camera.h Shader.h Mesh.h )
set(SOURCES Camera.cpp Shader.cpp Main.cpp Mesh.cpp )

set(CMAKE_BUILD_TYPE Debug FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

add_library(glew32 STATIC IMPORTED)
add_library(glfw3 STATIC IMPORTED)
add_library(assimp STATIC IMPORTED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} "../include")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

if(WIN32)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "..\\..\\lib\\Win32\\glew32s.lib")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "..\\..\\lib\\Win32\\glfw3.lib")
    set_target_properties(assimp PROPERTIES IMPORTED_LOCATION "..\\..\\lib\\Win32\\assimp-vc140-mt.lib")

    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32 glfw3 assimp)
    
    set(BUILD_DIR "./Debug")

elseif(APPLE)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libGLEW.a")
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libglfw3.a")
    set_target_properties(assimp PROPERTIES IMPORTED_LOCATION "../../lib/Unix/libassimp.dylib")

    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -g")
    target_link_libraries(${PROJECT_NAME} glew32 glfw3 assimp)
    set(BUILD_DIR ".")
endif()