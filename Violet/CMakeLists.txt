cmake_minimum_required(VERSION 3.20)

project(Violet VERSION 0.0.1)

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

set(Violet_src 
	src/main.cpp
	src/violet_app.cpp
)

set(Violet_include 
	include/violet_app.hpp
)

set(Violet_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${STB_IMAGE_INCLUDE}/stb_image.cpp
  ${STB_IMAGE_INCLUDE}/stb_image.h
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
  ${ThirdParty_DIR}/json11/json11.cpp
  ${ThirdParty_DIR}/json11/json11.hpp
)

add_executable(Violet ${Violet_src} ${Violet_include} ${Violet_3rd})
target_link_libraries(Violet 
  glfw
  opengl32
)