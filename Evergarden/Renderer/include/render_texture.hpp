/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_texture.hpp
 * @brief   A API package for texture class.
 */

#ifndef H_RENDERER_TEXTURE
#define H_RENDERER_TEXTURE

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"

namespace evergarden {

    /*!
     * @brief A most Baisc Texture class for all platform.
     */
    class Texture {
    public:
        Texture();
        virtual ~Texture() = default;

    public:
        virtual uint32_t    _W()                                const = 0;
        virtual uint32_t    _H()                                const = 0;
        virtual uint32_t    _id()                               const = 0;
        virtual void        setData(void* data, uint32_t size)  const = 0;
        virtual std::string _File_Path()                        const = 0;
        virtual void        bind(uint32_t enter = 0)            const = 0;

    };

    /*!
     * @brief A Baisc Texture 2D class for all platform.
     */
    class Texture2D: public Texture {
    public:
        static std::shared_ptr<Texture2D>   create(const std::string& path, bool hdr = false);
        static std::shared_ptr<Texture2D>   create(uint32_t width, uint32_t height);
        virtual uint32_t                    _W()            const = 0;
        virtual uint32_t                    _H()            const = 0;
        virtual bool                        _not_empty()    const = 0;
    };
}

namespace evergarden {

    /*!
     * @brief A OpenGL bind Texture 2D class.
     */
    class Texture2D_OpenGL: public Texture2D {
    public:
        Texture2D_OpenGL(uint32_t w, uint32_t h);
        // Texture2D_OpenGL(std::string path, bool is_hdr=false);
        Texture2D_OpenGL(const std::string &path, bool is_hdr=false);
        ~Texture2D_OpenGL();

    public:
        uint32_t    _C()                                            const { return m_c; };
        uint32_t    _W()                                            const override { return m_w; };
        uint32_t    _H()                                            const override { return m_h; };
        uint32_t    _id()                                           const override {return m_id; };
        void        shape(uint32_t &w, uint32_t &h, uint32_t &c)    const { w=m_w, h=m_h, c=m_c; };
        std::string _File_Path()                                    const override { return m_file_path; };

    public:
        void        setData(void* data, uint32_t size)              const override;
        void        bind(uint32_t enter=0)                          const override;
        bool        _not_empty()                                    const override { return !m_file_path.empty(); } 

    private:
        uint32_t    m_w, m_h, m_c;///< channel may set to 4 by default.
        uint32_t    m_id = 0;
        GLenum      m_format;
        std::string m_file_path;
    };

}

#endif //! H_RENDERER_TEXTURE
