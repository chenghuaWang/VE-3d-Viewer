cmake_minimum_required(VERSION 3.20)

project(Evergarden_Renderer VERSION 0.0.1)

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
include_directories(${ThirdParty_DIR})
include_directories(${IMGUI_INCLUDE})
include_directories(${GLFW_INCLUDE})
include_directories(${GLAD_INCLUDE})
include_directories(${TINY_OBJ_INCLUDE})
include_directories(${GLM_INCLUDE})

file(GLOB_RECURSE Evergarden_Renderer_src src/*.cpp)
file(GLOB_RECURSE Evergarden_Renderer_include include/*.hpp)

set(Evergarden_Renderer_3rd 
  ${STB_IMAGE_INCLUDE}/stb_image.cpp
  ${STB_IMAGE_INCLUDE}/stb_image.h
  ${GLAD_INCLUDE}/glad/glad.h
  ${GLAD_INCLUDE}/KHR/khrplatform.h
  ${GLAD_SRC}/glad.c
  ${OBJ_LOADER_FILE}
)

# add_library(Evergarden_Renderer SHARED ${Evergarden_Renderer_src} ${Evergarden_Renderer_include} ${Evergarden_Renderer_3rd})
add_library(Evergarden_Renderer STATIC ${Evergarden_Renderer_src} ${Evergarden_Renderer_include} ${Evergarden_Renderer_3rd})

target_link_libraries(Evergarden_Renderer
  glfw
  opengl32
  glm_static
  Evergarden_Core
  Evergarden_App
  Evergarden_Editor
)