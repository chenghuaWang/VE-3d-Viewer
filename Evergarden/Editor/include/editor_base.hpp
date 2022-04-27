/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_base.hpp
 * @brief   The base file provide some basic define and macros
 */

#ifndef H_EDITOR_BASE_
#define H_EDITOR_BASE_

#ifdef WIN32
#pragma once
#endif //! WIN32

#include <memory>
#include <string>

#define EDITOR_PXSR(s)          EDITOR_PSR(s)
#define EDITOR_PSR(s)           #s 

#define JOIN_PATH(lhs, rhs)     (lhs + "/" + "rhs")
#define GET_ABSOLUTE_PATH(rhs)  (EDITOR_PXSR(VE_ROOT_DIR) + rhs)

#define MAIN_LAYER_NAME         "Main-Layer"
#define SCENE_LAYER_NAME        "Scene-Layer"
#define SE_LAYER_NAME           "ShaderEditor-Layer"

namespace evergarden {

} //! namespace evergarden

#endif //! H_EDITOR_BASE_