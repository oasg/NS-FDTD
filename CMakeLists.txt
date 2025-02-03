cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 20)
project(NS-FDTD VERSION 0.1.0 LANGUAGES C CXX)



add_subdirectory(third_party/glfw)
add_subdirectory(third_party/json)
add_subdirectory(third_party/gtest)
add_subdirectory(third_party/spdlog)

file(GLOB SRC 
    src/* 
    src/type/*
    src/model/*
)

file (
  GLOB GUI src/gui/*
  third_party/glad/src/glad.c
)

include_directories(
    src
    third_party/glad/include
    third_party/imgui
    third_party/imguifile
    )


find_package(OpenGL REQUIRED)
if(APPLE)
    message(STATUS "Building on macOS")
    # macOS-specific settings
    # use homebrew-installed libomp
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
else()
    message(STATUS "Not macOS")
endif()


find_package(OpenMP REQUIRED)

add_executable(NS-FDTD 
    ${SRC}
    ${GUI}
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui.cpp
    third_party/imguifile/ImGuiFileDialog.cpp
    
)
target_link_libraries(NS-FDTD 
  OpenGL::GL
  glfw
  nlohmann_json
  OpenMP::OpenMP_CXX
  spdlog
)


add_executable(
  NS-FDTD-CLI
  ${SRC}
  src/cli/main.cpp
)
target_link_libraries(NS-FDTD-CLI 
  nlohmann_json
  OpenMP::OpenMP_CXX
  spdlog
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(NS-FDTD-CLI PRIVATE -O3 -march=native)
endif()

###
### Tests
###
enable_testing()
add_executable(
  model_parser_test
  tests/test_model_phase.cpp
  src/model/model_builder.cpp
  src/model/FazzyModel.cpp
  src/type/Field.cpp
  src/model/HairMultilayerModel.cpp
  src/model/HairMultilayerDamagedModel.cpp
  src/model/HairMultilayerRepairedModel.cpp
  src/model/HairPretteMultilayerModel.cpp
)



target_link_libraries(
  model_parser_test
  nlohmann_json
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(model_parser_test)