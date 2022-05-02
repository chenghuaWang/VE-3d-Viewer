/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_texture.cpp
 * @brief   A API package for texture class.
 */
#include "../include/render_texture.hpp"


namespace evergarden {
    ///< Texture created
    std::shared_ptr<Texture2D> Texture2D::create(const std::string& path, bool hdr) {
        return std::make_shared<Texture2D_OpenGL>(path, hdr);
    }
    
    std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
        return std::make_shared<Texture2D_OpenGL>(width, height);
    }

    ///< Under for OpenGL based implement.
    Texture2D_OpenGL::Texture2D_OpenGL(uint32_t w, uint32_t h):m_w(w), m_h(h), m_c(4) {
        // m_c is RGB and alpha channel. So 4 channel contains.
        GLenum internalFormat = GL_RGBA8;
        m_format = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, 1, internalFormat, m_w, m_h);

        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture2D_OpenGL::Texture2D_OpenGL(const std::string &path, bool is_hdr): m_file_path(path) {
        int tmp_width, tmp_height, tmp_channels;
        stbi_set_flip_vertically_on_load(1);
        if (!is_hdr) {
            stbi_uc* data = stbi_load(path.c_str(), &tmp_width, &tmp_height, &tmp_channels, 0);

            if (data == nullptr){
                std::cout << "Texture data load filed: " << path << std::endl;
            }

            m_w = tmp_width;
            m_h = tmp_height;

            GLenum internalFormat = 0;

            switch (tmp_channels) {
                case 1:
                    m_format = GL_RED;
                    internalFormat = GL_RED;
                    break;
                case 3:
                    m_format = GL_RGB;
                    internalFormat = GL_RGB8;
                    break;
                case 4:
                    m_format = GL_RGBA;
                    internalFormat = GL_RGBA8;
                    break;
                default:
                    std::cout << "Unsupported Image channel num: " << tmp_channels <<std::endl;
                    m_format = 0;
                    break;
            }

            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);
            glTexImage2D(GL_TEXTURE_2D, 0, m_format, tmp_width, tmp_height, 0, m_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);

        } else {
            float* data = stbi_loadf(path.c_str(), &tmp_width, &tmp_height, &tmp_channels, 0);

            if (data) {
                glGenTextures(1, &m_id);
                glBindTexture(GL_TEXTURE_2D, m_id);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, tmp_width, tmp_height, 0, GL_RGB, GL_FLOAT, data);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            } else {
                std::cout << "Texture data load filed: " << path << std::endl;
            }
        }
    }

    Texture2D_OpenGL::~Texture2D_OpenGL() {
        // delete the texture in Opengl state machine memory. In main DRAM or graphic cards memory.
        glDeleteTextures(1, &m_id);
    }

    void Texture2D_OpenGL::setData(void* data, uint32_t size) const {
        glTextureSubImage2D(m_id, 0, 0, 0, m_w, m_w, m_format, GL_UNSIGNED_BYTE, data);
    }

    void Texture2D_OpenGL::bind(uint32_t enter) const {
        glBindTextureUnit(enter, m_id);
    }

}
