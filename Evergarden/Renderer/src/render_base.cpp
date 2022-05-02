/*!
 * @author  chenghua.wang
 * @file    Evergarden/Renderer/include/render_base.hpp
 * @brief   Base file for Renderer. Provide macros and basic functions
 * 
 * @details We packagged OpenGL API in this Class-like interface.
 */

#define STB_IMAGE_IMPLEMENTATION
#include "../include/render_base.hpp"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[Opengl Error] (" << error << "):" << function << " " <<
                  file << ":" << line << std::endl;
        return false;
    }
    return true;
}

std::string __get_abs_path__(std::string a){
    return RENDER_XSR(VE_ROOT_DIR) + a;
}
