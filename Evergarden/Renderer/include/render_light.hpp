/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_light.hpp
 * @brief   A API package for light class.
 */

#ifndef H_RENDER_LIGHT
#define H_RENDER_LIGHT

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_shader.hpp"
#include <glm/glm.hpp>

namespace evergarden{

    enum class LightType {
        None    = 0,
        Default = 1,
        Direct  = 2,
        Point   = 3,
        Spot    = 4
    };

    static std::unordered_map<LightType, std::string> LightTypeMap = {
        {LightType::None,       "None"},
        {LightType::Default,    "Basic"},
        {LightType::Direct,     "DirLight"},
        {LightType::Point,      "PointLight"},
        {LightType::Spot,       "SpotLight"}
    };

    class Light {
    public:
        Light()         = default;
        ~Light()        = default;

    public:

        glm::vec3       m_colour{1.f, 1.f, 1.f};
        float           m_intensity = 1.f;

    public:
        virtual void bind(const std::shared_ptr<Shader>& shader, const glm::vec3& position, const uint32_t& enter) {
            shader->bind();
            shader->setVec3("cpu_Light["    + std::to_string(enter) + "].color",        m_colour);
            shader->setVec3("cpu_Light["    + std::to_string(enter) + "].position",     position);
            shader->setFloat("cpu_Light["   + std::to_string(enter) + "].intensity",    m_intensity);
        }
    };

    class DirectLight: public Light {
    public:
        DirectLight()   = default;
        ~DirectLight()  = default;

    public:
        glm::vec3 m_direction{1.f, 1.f, 1.f};
        LightType type = LightType::Direct;

    public:
        void bind(const std::shared_ptr<Shader>& shader, const glm::vec3& position, const uint32_t& enter) override {
            shader->bind();
            shader->setVec3("cpu_DirLight["     + std::to_string(enter) + "].color",        m_colour);
            shader->setVec3("cpu_DirLight["     + std::to_string(enter) + "].direction",    m_direction);
            shader->setFloat("cpu_DirLight["    + std::to_string(enter) + "].intensity",    m_intensity);
        }
    };

    class PointLight: public Light {
    public:
        PointLight()    = default;
        ~PointLight()   = default;

    public:
        float m_constant = 1.0f;
        float m_linear = 0.09f;
        float m_quadratic = 0.032f;
        LightType type = LightType::Point;

    public:
        void bind(const std::shared_ptr<Shader>& shader, const glm::vec3& position, const uint32_t& enter) override {
            shader->bind();
            shader->setVec3("cpu_PointLight["   + std::to_string(enter) + "].color",        m_colour);
            shader->setVec3("cpu_PointLight["   + std::to_string(enter) + "].position",     position);
            shader->setFloat("cpu_PointLight["  + std::to_string(enter) + "].constant",     m_constant);
            shader->setFloat("cpu_PointLight["  + std::to_string(enter) + "].linear",       m_linear);
            shader->setFloat("cpu_PointLight["  + std::to_string(enter) + "].quadratic",    m_quadratic);
            shader->setFloat("cpu_PointLight["  + std::to_string(enter) + "].intensity",    m_intensity);
        }
    };

    class SpotLight: public Light {
    public:
        SpotLight()     = default;
        ~SpotLight()    = default;

    public:
        glm::vec3  m_direction{1.f, 1.f, 1.f};
        float m_cutoff          = glm::cos(glm::radians(12.5f));
        float m_outer_cutoff    = glm::cos(glm::radians(17.5f));
        LightType type          = LightType::Spot;

    public:
        void bind(const std::shared_ptr<Shader>& shader, const glm::vec3& position, const uint32_t& enter) override {
            shader->bind();
            shader->setVec3("cpu_SpotLight["    + std::to_string(enter) + "].color",        m_colour);
            shader->setVec3("cpu_SpotLight["    + std::to_string(enter) + "].position",     position);
            shader->setVec3("cpu_SpotLight["    + std::to_string(enter) + "].direction",    m_direction);
            shader->setFloat("cpu_SpotLight["   + std::to_string(enter) + "].cutoff",       m_cutoff);
            shader->setFloat("cpu_SpotLight["   + std::to_string(enter) + "].outerCutOff",  m_outer_cutoff);
            shader->setFloat("cpu_SpotLight["   + std::to_string(enter) + "].intensity",    m_intensity);
        }

    };

} // namespace evergarde


#endif //! H_RENDER_LIGHT