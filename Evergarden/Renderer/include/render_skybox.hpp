/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_skybox.hpp
 * @brief   A API package for skybox class.
 */

#ifndef H_REDNER_SKYBOX
#define H_REDNER_SKYBOX

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_texture.hpp"
#include "render_camera.hpp"
#include "render_shader.hpp"

namespace evergarden {

    class SkyBox {
    public:
        virtual ~SkyBox() = default;

    public:
        virtual uint32_t                    getSkyBoxID()       const = 0;
        virtual uint32_t                    getTextureID()      const = 0;
        virtual std::string                 _File_Path()        const = 0;
        virtual std::shared_ptr<Texture2D>& getTexture() = 0;

    public:
        virtual void bindSkyBox(Camera& camera, uint32_t enter=0)  const = 0;
        virtual void bindIrradianceMap(uint32_t enter=0)           const = 0;
        virtual void bindPrefilterMap(uint32_t enter=0)            const = 0;
        virtual void bindBRDFLUTTMap(uint32_t enter=0)             const = 0;

    public:
        static std::shared_ptr<SkyBox>  create(const std::string& path);
        static std::shared_ptr<SkyBox>  create();
    };

    class SkyBox_OpenGL: public SkyBox {
    public:
        SkyBox_OpenGL();
        SkyBox_OpenGL(const std::string &FilePath);
        ~SkyBox_OpenGL();

    public:
        uint32_t                    getSkyBoxID()   const   override { return m_SkyBoxID; }
        uint32_t                    getTextureID()  const   override { return m_Texture->_id(); }
        std::string                 _File_Path()    const   override { return m_Texture->_File_Path(); }
        std::shared_ptr<Texture2D>& getTexture()            override { return m_Texture; };

        void bindSkyBox(Camera& camera, uint32_t enter=0)   const override;
        void bindIrradianceMap(uint32_t enter=0)            const override;
        void bindPrefilterMap(uint32_t enter=0)             const override;
        void bindBRDFLUTTMap(uint32_t enter=0)              const override;

    private:
        std::shared_ptr<Texture2D>  m_Texture;
        std::shared_ptr<Shader>     m_Eq2SkyBoxShader;
        std::shared_ptr<Shader>     m_BackgroundShader;
        std::shared_ptr<Shader>     m_IrradianceShader;
        std::shared_ptr<Shader>     m_PrefilterShader;
        std::shared_ptr<Shader>     m_BRDFShader;
        uint32_t                    m_SkyBoxID = 0;
        uint32_t                    m_IrradianceMapID = 0;
        uint32_t                    m_PrefilterMapID = 0;
        uint32_t                    m_BRDFLUTTextureID = 0;
        uint32_t                    m_CubeVAO = 0;
        uint32_t                    m_CubeVBO = 0;
        uint32_t                    m_QuadVAO = 0;
        uint32_t                    m_QuadVBO = 0;

    private:
        void GenCube();
        void GenQuad();
        void GenBuffer();
        void RenderCube() const;
        void RenderQuad() const;
    };

} //! namespace evergarden

#endif //! H_REDNER_SKYBOX
