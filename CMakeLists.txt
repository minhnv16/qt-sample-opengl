cmake_minimum_required(VERSION 3.5)

project(qt-sample-console LANGUAGES CXX)







set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (UNIX)


    set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glfw-3.3.4-ubuntu")
    set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glew-2.2.0-ubuntu")



    include_directories("${glfw_pre_built_dir}/include")
    include_directories("${glew_pre_built_dir}/include")

    add_library(lib_glfw STATIC IMPORTED)
    set_target_properties( lib_glfw
            PROPERTIES IMPORTED_LOCATION
            ${glfw_pre_built_dir}/lib/libglfw3.a)

    add_library(lib_glew SHARED IMPORTED)
    set_target_properties( lib_glew
            PROPERTIES IMPORTED_LOCATION
            ${glew_pre_built_dir}/lib/libGLEW.a)

    find_package(OpenGL REQUIRED)

    find_package(X11 REQUIRED)
    find_package (Threads REQUIRED)
    #find_package(GL REQUIRED)
    find_package(GLU REQUIRED)
    find_package(GLUT REQUIRED)


endif (UNIX)


if (WIN32)

    set(glfw_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glfw-3.3.4-mingw64")
    set(glew_pre_built_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext_lib/glew-2.2.0-mingw64")

    include_directories("${glfw_pre_built_dir}/include")
    include_directories("${glew_pre_built_dir}/include")


    add_library(lib_glew SHARED IMPORTED)
    set_target_properties( lib_glew
            PROPERTIES  IMPORTED_IMPLIB
            ${glew_pre_built_dir}/lib/libglew32.dll.a)


    add_library(lib_glfw SHARED IMPORTED)
    set_target_properties( lib_glfw
            PROPERTIES  IMPORTED_IMPLIB
            ${glfw_pre_built_dir}/lib/libglfw3.a)
    find_package(OpenGL REQUIRED)

endif (WIN32)




add_executable(
    qt-sample-console
    main.cpp
    WindowCreator.cpp
    WindowCreator.h
    Source2_sample_drawing_triangle_index.cpp
    Learn6.cpp
    Learn6_transform.cpp
    )

if (WIN32)


target_link_libraries(
    qt-sample-console
    lib_glew
    lib_glfw
    ${OPENGL_LIBRARY}
)

endif (WIN32)

if (UNIX)
target_link_libraries(
    qt-sample-console
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
