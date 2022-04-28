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


file(GLOB_RECURSE Evergarden_Core src/*.cpp)
file(GLOB_RECURSE Evergarden_Core include/*.hpp)

set(Evergarden_Core_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
)

add_library(Evergarden_Core SHARED ${Evergarden_Core_src} ${Evergarden_Core_include} ${Evergarden_Core_3rd})

target_link_libraries(Violet 
  glfw
  opengl32
)