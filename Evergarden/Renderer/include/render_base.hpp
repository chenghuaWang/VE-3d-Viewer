/*!
 * @author  chenghua.wang
 * @file    Evergarden/Renderer/include/render_base.hpp
 * @brief   Base file for Renderer. Provide macros and basic functions
 * 
 * @details We packagged OpenGL API in this Class-like interface.
 */

#ifndef H_RENDER_BASE
#define H_RENDER_BASE

#ifdef WIN32
#pragma once
#endif //! WIN32

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include <stb_image/stb_image.h>

#include <memory>

#include <cinttypes>

#ifdef WIN32
#define ASSERT(x) {if(!(x)) __debugbreak();}
#else
#define ASSERT(x) {if(!(x)) asm("int $3");}
#endif

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif

#define RENDER_XSR(s) RENDER_SR(s)
#define RENDER_SR(s) #s

std::string __get_abs_path__(std::string a);