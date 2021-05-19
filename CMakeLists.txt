cmake_minimum_required(VERSION 3.5)

project(qt-sample-console LANGUAGES CXX)

if (WIN32)
    #do something
    #hihi
endif (WIN32)





set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(qt-sample-console main.cpp WindowCreator.cpp WindowCreator.h)

if (UNIX)

    set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glfw-3.3.4-ubuntu")
    set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glew-2.2.0-ubuntu")



    include_directories("${glfw_pre_built_dir}/include")
    include_directories("${glew_pre_built_dir}/include")

    add_library(lib_glfw SHARED IMPORTED)
    set_target_properties( lib_glfw
            PROPERTIES IMPORTED_LOCATION
            ${glfw_pre_built_dir}/lib/libglfw3.a)

    add_library(lib_glew SHARED IMPORTED)
    set_target_properties( lib_glew
            PROPERTIES IMPORTED_LOCATION
            ${glew_pre_built_dir}/lib/libGLEW.a)



endif (UNIX)

target_link_libraries(
                       qt-sample-console
                       lib_glfw
                       lib_glew

                     )
