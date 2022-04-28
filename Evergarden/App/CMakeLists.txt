cmake_minimum_required(VERSION 3.20)

project(Evergarden VERSION 0.0.1)

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

include_directories(${V_CORE_INCLUDE})
include_directories(${V_EDITOR_INCLUDE})

set(Evergarden_src 
	src/app_window.cpp
	src/Application.cpp
)

set(Evergarden_include 
	include/app_window.hpp
    include/Application.hpp
)

set(Evergarden_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
)

add_executable(Evergarden ${Evergarden_src} ${Evergarden_include} ${Evergarden_3rd})
target_link_libraries(Evergarden 
  glfw
  opengl32
  Evergarden_Core
  Evergarden_Editor
)