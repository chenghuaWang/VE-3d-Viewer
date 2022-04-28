/*!
 * @author  chenghua.wang
 * @file    Evergarden/Core/include/core_base.hpp
 * @brief   base macros and functions
 */

#ifndef H_CORE_EVENT_BASE
#define H_CORE_EVENT_BASE

#ifdef WIN32
#pragma once
#endif //! WIN32

#ifdef _MSC_VER
typedef signed char int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned char uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <inttypes.h>
#endif

#include <iostream>
#include <string>
#include <functional>
#include <sstream>

#define BIT(x) (1<<x)

namespace evergarden {

}

#endif //! H_CORE_EVENT_BASE