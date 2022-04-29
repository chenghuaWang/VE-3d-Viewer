cmake_minimum_required(VERSION 3.20)

project(Evergarden_Core VERSION 0.0.1)

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

file(GLOB_RECURSE Evergarden_Core src/*.cpp)
file(GLOB_RECURSE Evergarden_Core include/*.hpp)

set(Evergarden_Core_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
)

# add_library(Evergarden_Core SHARED ${Evergarden_Core_src} ${Evergarden_Core_include} ${Evergarden_Core_3rd})
add_library(Evergarden_Core STATIC ${Evergarden_Core_src} ${Evergarden_Core_include} ${Evergarden_Core_3rd})

target_link_libraries(Evergarden_Core 
  glfw
  opengl32
)
