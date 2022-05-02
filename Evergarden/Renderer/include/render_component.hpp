/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_component.hpp
 * @brief   A part of entity component system.
 */

#ifndef H_RENDER_COMPONENT
#define H_RENDER_COMPONENT

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "render_base.hpp"
#include "render_light.hpp"
#include "render_model.hpp"
#include "render_camera.hpp"
#include "render_shader.hpp"
#include "render_skybox.hpp"
#include "render_material.hpp"

///< Everything here is a component.
namespace evergarden {

    class TagComponent {
    public:
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(std::string tag): Tag(std::move(tag)) {}

    public:
        std::string Tag;
    };

    class TransformComponent {
    public:
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
                :Position(position), Rotation(rotation), Scale(scale) {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position + RelatePosition);
            glm::mat4 rotation_ = glm::mat4_cast(glm::qua<float>(glm::radians(Rotation)));
            Transform = glm::scale(translation * rotation_, Scale);
        }

    public:
        void Update() {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position + RelatePosition);
            glm::mat4 rotation = glm::mat4_cast(glm::qua<float>(glm::radians(Rotation)));
            Transform = glm::scale(translation * rotation, Scale);
        }

        void SetRelatePosition(const glm::vec3& position) {
            Position = Position + RelatePosition;
            RelatePosition = position;
            Position = Position - position;
        }

    public:
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }

    public:
        glm::vec3 Position{ 0.0f };
        glm::vec3 Rotation{ 0.0f };
        glm::vec3 Scale{ 1.0f };

        glm::mat4 Transform{ 1.0f };
        glm::vec3 RelatePosition{ 0.0f };
    };

    class MaterialComponent {
    public:
        MaterialComponent() = default;
        MaterialComponent(MaterialType type):Type(type) {
            switch (type) {
                case MaterialType::None:
                    MaterialSrc = std::make_shared<Material>();
                    break;
                case MaterialType::Emission:
                    MaterialSrc = std::make_shared<EmissionMaterial>();
                    break;
                case MaterialType::Phong:
                    MaterialSrc = std::make_shared<PhongMaterial>();
                    break;
                case MaterialType::BRDF_CT:
                    MaterialSrc = std::make_shared<CT_BRDF>();
                    break;
                default:
                    break;
            }
        }

    public:
        void SetMaterialType(MaterialType type) {
            MaterialSrc.reset();
            Type = type;
            switch (type) {
                case MaterialType::None:
                    MaterialSrc = std::make_shared<Material>();
                    break;
                case MaterialType::Emission:
                    MaterialSrc = std::make_shared<EmissionMaterial>();
                    break;
                case MaterialType::Phong:
                    MaterialSrc = std::make_shared<PhongMaterial>();
                    break;
                case MaterialType::BRDF_CT:
                    MaterialSrc = std::make_shared<CT_BRDF>();
                    break;
                default:
                    break;
            }
        }

        void ResetType() {
            MaterialSrc.reset();
            switch (Type) {
                case MaterialType::None:
                    MaterialSrc = std::make_shared<Material>();
                    break;
                case MaterialType::Emission:
                    MaterialSrc = std::make_shared<EmissionMaterial>();
                    break;
                case MaterialType::Phong:
                    MaterialSrc = std::make_shared<PhongMaterial>();
                    break;
                case MaterialType::BRDF_CT:
                    MaterialSrc = std::make_shared<CT_BRDF>();
                    break;
                default:
                    break;
            }
        }

        void bind(const std::shared_ptr<SkyBox>& envMap = nullptr) {
            switch (Type) {
                case MaterialType::None:
                    MaterialSrc->bind();
                    break;
                case MaterialType::Emission:
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->bind();
                    break;
                case MaterialType::Phong:
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->bind();
                    break;
                case MaterialType::BRDF_CT:
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->bind(envMap);
                    break;
                default:
                    break;
            }
        }

        void AddLight(LightType type) {
            LightCount[type]++;
            switch (Type) {
                case MaterialType::None:
                    MaterialSrc->bind();
                    MaterialSrc->m_Shader->setInt(LightTypeMap[type] + "Num", LightCount[type]);
                    break;
                case MaterialType::Emission:
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->bind();
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader->setInt(LightTypeMap[type] + "Num", LightCount[type]);
                    break;
                case MaterialType::Phong:
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->bind();
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader->setInt(LightTypeMap[type] + "Num", LightCount[type]);
                    break;
                case MaterialType::BRDF_CT:
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->bind();
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader->setInt(LightTypeMap[type] + "Num", LightCount[type]);
                    break;
                default:
                    break;
            }
        }

        void ResetLight() {
            for (auto& light : LightCount) {
                light.second = 0;
                switch (Type) {
                    case MaterialType::None:
                        MaterialSrc->bind();
                        MaterialSrc->m_Shader->setInt(LightTypeMap[light.first] + "Num", 0);
                        break;
                    case MaterialType::Emission:
                        std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->bind();
                        std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader->setInt(LightTypeMap[light.first] + "Num", 0);
                        break;
                    case MaterialType::Phong:
                        std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->bind();
                        std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader->setInt(LightTypeMap[light.first] + "Num", 0);
                        break;
                    case MaterialType::BRDF_CT:
                        std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->bind();
                        std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader->setInt(LightTypeMap[light.first] + "Num", 0);
                        break;
                    default:
                        break;
                }
            }
        }

        void Set(Camera& camera, glm::mat4& transform) {
            switch (Type) {
                case MaterialType::None:
                    MaterialSrc->m_Shader->setMat4("u_ViewProjection", camera._ViewProjectionMatrix());
                    MaterialSrc->m_Shader->setMat4("u_Transform", transform);
                    return;
                case MaterialType::Emission: {
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader->setMat4("cpu_ViewProjection", camera._ViewProjectionMatrix());
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader->setMat4("cpu_Transform", transform);
                    std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader->setVec3("cpu_ViewPos", camera._Position());
                    return;
                }
                case MaterialType::Phong:
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader->setMat4("cpu_ViewProjection", camera._ViewProjectionMatrix());
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader->setMat4("cpu_Transform", transform);
                    std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader->setVec3("cpu_ViewPos", camera._Position());
                    return;
                case MaterialType::BRDF_CT:
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader->setMat4("cpu_ViewProjection", camera._ViewProjectionMatrix());
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader->setMat4("cpu_Transform", transform);
                    std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader->setVec3("cpu_ViewPos", camera._Position());
                    return;
                default:
                    break;
            }
        }

        std::shared_ptr<Shader>& GetShader() const {
            switch (Type) {
                case MaterialType::None:
                    return MaterialSrc->m_Shader;
                case MaterialType::Emission:
                    return std::dynamic_pointer_cast<EmissionMaterial>(MaterialSrc)->m_Shader;
                case MaterialType::Phong:
                    return std::dynamic_pointer_cast<PhongMaterial>(MaterialSrc)->m_Shader;
                case MaterialType::BRDF_CT:
                    return std::dynamic_pointer_cast<CT_BRDF>(MaterialSrc)->m_Shader;
                default:
                    return MaterialSrc->m_Shader;
                    break;
            }
        }

    public:
        std::shared_ptr<Material> MaterialSrc = std::make_shared<Material>();
        MaterialType Type = MaterialType::None;
        std::unordered_map<LightType, uint32_t> LightCount = {
                {LightType::None, 0},
                {LightType::Default, 0},
                {LightType::Direct, 0},
                {LightType::Point, 0},
                {LightType::Spot, 0}
        };
    };

    class MeshComponent {
    public:
        void Reset() { c_Mesh.reset(); c_Mesh = Mesh::Create(); }
        void Reload(const std::string& path) {
            c_Mesh.reset();
            c_Mesh = Mesh::Create(path);
        }

        void Reload(const MeshType& type) {
            c_Mesh.reset();
            c_Mesh = Mesh::Create(type);
        }

    public:
        std::shared_ptr<Mesh> c_Mesh = std::make_shared<Mesh>();

    public:
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(const std::shared_ptr<Mesh>& mesh):c_Mesh(mesh) {}
        MeshComponent(const std::string& path):c_Mesh(Mesh::Create(path)) {}
        MeshComponent(const MeshType& type):c_Mesh(Mesh::Create(type)) {}

    };

    struct LightComponent {
        std::shared_ptr<Light> LightSrc = std::make_shared<Light>();
        LightType Type = LightType::Default;

        LightComponent() = default;

        void SetLight(LightType type) {
            LightSrc.reset();
            switch (type) {
                case LightType::None:
                    LightSrc = nullptr;
                    break;
                case LightType::Default:
                    LightSrc = std::make_shared<Light>();
                    break;
                case LightType::Direct:
                    LightSrc = std::make_shared<DirectLight>();
                    break;
                case LightType::Point:
                    LightSrc = std::make_shared<PointLight>();
                    break;
                case LightType::Spot:
                    LightSrc = std::make_shared<SpotLight>();
                    break;
                default:
                    break;
            }
        }

        void ResetType() {
            LightSrc.reset();
            switch (Type) {
                case LightType::None:
                    LightSrc = nullptr;
                    break;
                case LightType::Default:
                    LightSrc = std::make_shared<Light>();
                    break;
                case LightType::Direct:
                    LightSrc = std::make_shared<DirectLight>();
                    break;
                case LightType::Point:
                    LightSrc = std::make_shared<PointLight>();
                    break;
                case LightType::Spot:
                    LightSrc = std::make_shared<SpotLight>();
                    break;
                default:
                    break;
            }
        }

        void SetType(LightType type) {
            LightSrc.reset();
            Type = type;
            switch (Type) {
                case LightType::None:
                    LightSrc = nullptr;
                    break;
                case LightType::Default:
                    LightSrc = std::make_shared<Light>();
                    break;
                case LightType::Direct:
                    LightSrc = std::make_shared<DirectLight>();
                    break;
                case LightType::Point:
                    LightSrc = std::make_shared<PointLight>();
                    break;
                case LightType::Spot:
                    LightSrc = std::make_shared<SpotLight>();
                    break;
                default:
                    break;
            }
        }

        void bind(const std::shared_ptr<Shader>& shader, const glm::vec3& position, const uint32_t& slot = 0) {
            switch (Type) {
                case LightType::None:
                    break;
                case LightType::Default:
                    LightSrc->bind(shader, position, slot);
                    break;
                case LightType::Direct:
                    std::dynamic_pointer_cast<DirectLight>(LightSrc)->bind(shader, position, slot);
                    break;
                case LightType::Point:
                    std::dynamic_pointer_cast<PointLight>(LightSrc)->bind(shader, position, slot);
                    break;
                case LightType::Spot:
                    std::dynamic_pointer_cast<SpotLight>(LightSrc)->bind(shader, position, slot);
                    break;
                default:
                    break;
            }
        }
    };

} //! namespace evergarden

#endif //! H_RENDER_COMPONENT
