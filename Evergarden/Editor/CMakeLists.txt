cmake_minimum_required(VERSION 3.20)

project(Evergarden_Editor VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In source builds not allowed. Please make a new directory and run CMake from there."
  )
endif()

add_compile_definitions("VE_ROOT_DIR=${VE_ROOT_DIR}")

include_directories(${ThirdParty_DIR})
include_directories(${IMGUI_INCLUDE})
include_directories(${GLFW_INCLUDE})
include_directories(${GLAD_INCLUDE})

file(GLOB_RECURSE Evergarden_Editor_src src/*.cpp)
file(GLOB_RECURSE Evergarden_Editor_include include/*.hpp)

set(Evergarden_Editor_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${STB_IMAGE_INCLUDE}/stb_image.cpp
  ${STB_IMAGE_INCLUDE}/stb_image.h
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
)

add_library(Evergarden_Editor SHARED ${Evergarden_Editor_src} ${Evergarden_Editor_include} ${Evergarden_Editor_3rd})

target_link_libraries(Evergarden_Editor
  glfw
  opengl32
)