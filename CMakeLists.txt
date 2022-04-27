cmake_minimum_required(VERSION 3.20)

project(VioletEvergarden VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In source builds not allowed. Please make a new directory and run CMake from there."
  )
endif()

set(VE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX "${VE_ROOT_DIR}/bin")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")

set(ThirdParty_DIR ${PROJECT_SOURCE_DIR}/3rdparty/)
set(GLFW_INCLUDE ${ThirdParty_DIR}/glfw/include/)
set(IMGUI_INCLUDE ${ThirdParty_DIR}/imgui/)
set(GLAD_INCLUDE ${ThirdParty_DIR}/glad/include/)
set(GLAD_SRC ${ThirdParty_DIR}/glad/src/)
set(ASSIMP_INCLUDE ${ThirdParty_DIR}/assimp/include/)
set(GLM_INCLUDE ${ThirdParty_DIR}/glm/)
set(STB_IMAGE_INCLUDE ${ThirdParty_DIR}/stb_image)
set(TINY_OBJ_INCLUDE ${ThirdParty_DIR}/tinyobjloader)
set(ENTT_INCLUDE ${ThirdParty_DIR}/entt/)
file(GLOB_RECURSE IMG_SRC ${IMGUI_INCLUDE}/*.cpp)
file(GLOB_RECURSE IMG_HDR ${IMGUI_INCLUDE}/*.h)
set (OBJ_LOADER_FILE ${TINY_OBJ_INCLUDE}/tiny_obj_loader.cc ${TINY_OBJ_INCLUDE}/tiny_obj_loader.h)


add_subdirectory(3rdparty/glfw)

add_subdirectory(Violet)
add_subdirectory(Evergarden/Editor)
