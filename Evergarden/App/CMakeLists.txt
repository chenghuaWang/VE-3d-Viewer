cmake_minimum_required(VERSION 3.20)

project(Evergarden_App VERSION 0.0.1)

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

include_directories(${V_DIR})
include_directories(${V_CORE_INCLUDE})
include_directories(${V_EDITOR_INCLUDE})

include_directories(${ThirdParty_DIR})
include_directories(${IMGUI_INCLUDE})
include_directories(${GLFW_INCLUDE})
include_directories(${GLAD_INCLUDE})

set(Evergarden_App_src 
	src/Application.cpp
)

set(Evergarden_App_include 
  include/Application.hpp
)

set(Evergarden_App_3rd 
  ${IMG_SRC} 
  ${IMG_HDR}
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
)

# add_library(Evergarden_App SHARED ${Evergarden_App_src} ${Evergarden_App_include} ${Evergarden_App_3rd})
add_library(Evergarden_App STATIC ${Evergarden_App_src} ${Evergarden_App_include} ${Evergarden_App_3rd})
target_link_libraries(Evergarden_App 
  glfw
  opengl32
  Evergarden_Core
  Evergarden_Editor
  Evergarden_Renderer
)