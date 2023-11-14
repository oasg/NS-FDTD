cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
project(NS-FDTD VERSION 0.1.0 LANGUAGES C CXX)
add_subdirectory(third_party/glfw)
file(GLOB SRC src/* 
            src/gui/*
            third_party/glad/src/glad.c)
include_directories(
    src
    src/gui
    third_party/glad/include
    third_party/imgui
    )


find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(NS-FDTD 
    ${SRC}
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui.cpp
)
target_link_libraries(NS-FDTD 
OpenGL::GL
glfw
OpenMP::OpenMP_CXX
)



