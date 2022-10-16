cmake_minimum_required(VERSION 3.5)
set(out_target MovingGame)

project(${out_target} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



if (UNIX)
    set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/out-glfw")
    set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/out-glew")
    set(libglfw_path "lib/libglfw3.a")
    set(libglew_path "lib/libGLEW.a")
elseif (WIN32)
    if(MSVC)
        set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/glfw-3.3.4-v142")
        set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/glew-2.2.0-v142")
        set(libglfw_path "x64/Release/glfw3.lib")
        set(libglew_path "x64/Release/glew32.lib")
    else()
        set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/glfw-3.3.4-mingw64")
        set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/../ext_lib/glew-2.2.0-mingw64")
        set(libglfw_path "lib/libglfw3.a")
        set(libglew_path "lib/libglew32.dll.a")
    endif (MSVC)
endif (UNIX)


include_directories("${glfw_pre_built_dir}/include")
include_directories("${glew_pre_built_dir}/include")

add_library(lib_glfw STATIC IMPORTED)
set_target_properties( lib_glfw
        PROPERTIES IMPORTED_LOCATION
        ${glfw_pre_built_dir}/${libglfw_path})

add_library(lib_glew STATIC IMPORTED)
set_target_properties( lib_glew
        PROPERTIES IMPORTED_LOCATION
        ${glew_pre_built_dir}/${libglew_path})

find_package(OpenGL REQUIRED)
if (UNIX)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    #find_package(GLUT REQUIRED)
elseif (WIN32)
endif (UNIX)


add_executable(MovingGame main.cpp
gamerender.h gamerender.cpp
gamemanager.h gamemanager.cpp
gamecontroller.h gamecontroller.cpp
gamemodel.h gamemodel.cpp
common.h common.cpp
)



if (WIN32)
target_link_libraries(
    ${out_target}
    lib_glew
    lib_glfw
    ${OPENGL_LIBRARY}
)

endif (WIN32)

if (UNIX)
target_link_libraries(
    ${out_target}
    lib_glew
    lib_glfw
    ${X11_LIBRARIES}
    ${OPENGL_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${GL_LIBRARIES}
    ${GLU_LIBRARIES}
    ${GLUT_LIBRARIES}
)
endif (UNIX)

