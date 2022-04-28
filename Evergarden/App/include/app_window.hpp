/*!
 * @author  chenghua.wang
 * @file    Evergarden/App/include/app_window.hpp
 * @brief   provide a basic virtual codes for multi-platform codes.
 */

#ifndef H_APP_WINDOW
#define H_APP_WINDOW

#ifdef WIN32
#pragma once
#endif //! WIN32

#include <string>

namespace evergarden {

    class MainWindowPayload {
    public:
        MainWindowPayload(): m_W(1920), m_H(1080), m_title("Evergarden") {};
        MainWindowPayload(unsigned int w, unsigned int h, std::string &title): m_W(w), m_H(h), m_title(title) {};
        ~MainWindowPayload() = default;

    public:
        unsigned int    m_W, m_H;
        std::string     m_title;

    };

    

} //! evergarden

#endif //! H_APP_WINDOW
