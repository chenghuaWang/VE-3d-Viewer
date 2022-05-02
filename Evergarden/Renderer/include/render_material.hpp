/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_material.hpp
 * @brief   A API package for material class.
 */

#ifndef H_REDNER_MATERIAL
#define H_REDNER_MATERIAL

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_skybox.hpp"
#include "render_shader.hpp"
#include "render_texture.hpp"

namespace evergarden {

    enum class MaterialType {
        None        = 0,
        Emission    = 1,
        Phong       = 2,
        BRDF_CT     = 3 ///< Cook_Torrance BRDF
    };

    class Material {
    public:
        Material() = default;
        virtual ~Material() {}
    
    public:
        virtual void bind(const std::shared_ptr<SkyBox>& envMap = nullptr) { m_Shader->bind(); };
        virtual void resetTexture(std::shared_ptr<Texture2D>& texture, const std::string path = "") {
            if (path.empty()) {
                texture = Texture2D::create(1, 1);
                uint32_t data = 0xffffffff;
                texture->setData(&data, sizeof(uint32_t));
            } else {
                texture.reset();
                texture = Texture2D::create(path);
            }
        }
    
    public:
        ///< TODO
        std::shared_ptr<Shader> m_Shader = Shader::create("D:/Project/Nebula-Engine/Asset/shader/Grid.shader");
    };

    class EmissionMaterial :public Material {
    public:
        EmissionMaterial() = default;
        ~EmissionMaterial() override = default;

        void bind(const std::shared_ptr<SkyBox>& envMap = nullptr) override {
            m_Shader->bind();
            m_Shader->setVec3("cpu_m_EmissionColor", m_EmissionColor);
            m_Shader->setFloat("cpu_m_Intensity", m_Intensity);
        }

    public:
        glm::vec3               m_EmissionColor{1.f, 1.f, 1.f};
        float                   m_Intensity = 1.0f;
        // TODO
        std::shared_ptr<Shader> m_Shader = Shader::create("D:/Project/Nebula-Engine/Asset/shader/Emission.shader");
    };

    class PhongMaterial :public Material {
    public:
        PhongMaterial() {
            resetTexture(m_DiffuseTexture);
            resetTexture(m_SpecularTexture);
            resetTexture(m_NormalTexture);
            resetTexture(m_DisplacementTexture);
        }
        virtual ~PhongMaterial() {}

        void resetTexture(std::shared_ptr<Texture2D>& texture, const std::string path = "") override {
            if (path.empty()) {
                texture = Texture2D::create(1, 1);
                uint32_t data = 0xffffffff;
                texture->setData(&data, sizeof(uint32_t));
            }
            else {
                texture.reset();
                texture = Texture2D::create(path);
            }
        }

        void bind(const std::shared_ptr<SkyBox>& envMap = nullptr) override {
            m_Shader->bind();
            m_DiffuseTexture->bind(0);
            m_Shader->setInt("cpu_Material.diffuse", 0);
            m_SpecularTexture->bind(1);
            m_Shader->setInt("cpu_Material.specular", 1);
            if (m_NormalTexture->_not_empty()) {
                m_Shader->setInt("cpu_UseNormalMap", 1);
            } else {
                m_Shader->setInt("cpu_UseNormalMap", 0);
            }
            m_NormalTexture->bind(2);
            m_Shader->setInt("cpu_Material.normalMap", 2);
            m_DisplacementTexture->bind(3);
            m_Shader->setInt("cpu_Material.displacementMap", 3);
            m_Shader->setVec3("cpu_Material.color", m_Color);
            m_Shader->setFloat("cpu_m_HeightScale", m_HeightScale);
            m_Shader->setFloat("cpu_Material.shininess", m_Shininess);
        }
    public:
        std::shared_ptr<Shader>     m_Shader = Shader::create("Default");
        std::shared_ptr<Texture2D>  m_DiffuseTexture = nullptr;
        std::shared_ptr<Texture2D>  m_SpecularTexture = nullptr;
        std::shared_ptr<Texture2D>  m_NormalTexture = nullptr;
        std::shared_ptr<Texture2D>  m_DisplacementTexture = nullptr;
        float                       m_HeightScale = 0;
        glm::vec3                   m_Color{1.f, 1.f, 1.f};
        float                       m_Shininess = 32;
    };

    class CT_BRDF :public Material {
    public:
        CT_BRDF() {
            resetTexture(AlbedoTexture);
            resetTexture(m_NormalTexture);
            resetTexture(MetallicTexture);
            resetTexture(RoughnessTexture);
            resetTexture(AOTexture);
            resetTexture(m_DisplacementTexture);
        }
        virtual ~CT_BRDF() {}
        void bind(const std::shared_ptr<SkyBox>& envMap = nullptr) override {
            m_Shader->bind();
            if (envMap) {
                envMap->bindIrradianceMap(0);
                envMap->bindPrefilterMap(1);
                envMap->bindBRDFLUTTMap(2);
                m_Shader->setInt("cpu_irradianceMap", 0);
                m_Shader->setInt("cpu_prefilterMap", 1);
                m_Shader->setInt("cpu_brdfLUT", 2);
            }
            else {
                m_Shader->setInt("cpu_irradianceMap", -1);
                m_Shader->setInt("cpu_prefilterMap", -1);
                m_Shader->setInt("cpu_brdfLUT", -1);
            }
            AlbedoTexture->bind(3);
            m_Shader->setInt("cpu_Material.albedoMap", 3);
            m_NormalTexture->bind(4);
            m_Shader->setInt("cpu_Material.normalMap", 4);
            MetallicTexture->bind(5);
            m_Shader->setInt("cpu_Material.metallicMap", 5);
            RoughnessTexture->bind(6);
            m_Shader->setInt("cpu_Material.roughnessMap", 6);
            AOTexture->bind(7);
            m_Shader->setInt("cpu_Material.aoMap", 7);
            m_DisplacementTexture->bind(8);
            m_Shader->setInt("cpu_Material.displacementMap", 8);
            if(m_NormalTexture->_not_empty()) {
                m_Shader->setInt("cpu_HasNormalMap", 1);
            }
            else m_Shader->setInt("cpu_HasNormalMap", 0);
            m_Shader->setVec3("cpu_Material.color", m_Color);
            m_Shader->setFloat("cpu_Material.metallic", m_Metallic);
            m_Shader->setFloat("cpu_Material.roughness", m_Roughness);
            m_Shader->setFloat("cpu_m_HeightScale", m_HeightScale);
        }

    public:
        glm::vec3                   m_Color{1.f, 1.f, 1.f};
        std::shared_ptr<Shader>     m_Shader = Shader::create("Default");
        std::shared_ptr<Texture2D>  AlbedoTexture = nullptr;
        std::shared_ptr<Texture2D>  m_NormalTexture = nullptr;
        std::shared_ptr<Texture2D>  MetallicTexture = nullptr;
        std::shared_ptr<Texture2D>  RoughnessTexture = nullptr;
        std::shared_ptr<Texture2D>  AOTexture = nullptr;
        std::shared_ptr<Texture2D>  m_DisplacementTexture = nullptr;
        float                       m_HeightScale = 0.0f;
        float                       m_Metallic = 1.0f;
        float                       m_Roughness = 1.0f;
    };

} //! evergarden

#endif //! H_REDNER_MATERIAL
