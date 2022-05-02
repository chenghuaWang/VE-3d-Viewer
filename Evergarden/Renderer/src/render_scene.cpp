/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_scene.cpp
 * @brief   A API package for scene class.
 */

#include "../include/render_scene.hpp"

namespace evergarden {

    void Scene::RenderObject(Camera &camera, const std::shared_ptr<SkyBox> &envMap) {
        RenderLight(camera);
        auto group = c_Registry.group<TagComponent, TransformComponent>(entt::get<>);

        for (auto entity : group) {
            auto& transform = group.get<TransformComponent>(entity);
            transform.Update();

            if (c_Registry.all_of<MaterialComponent>(entity) && !c_LineMode) {
                c_Registry.get<MaterialComponent>(entity).bind();
                auto& material = c_Registry.get<MaterialComponent>(entity);
                material.bind(envMap);
                material.Set(camera, transform);
            }
            else {
                c_DefaultShader->bind();
                c_DefaultShader->setMat4("cpu_ViewProjection", camera._ViewProjectionMatrix());
                c_DefaultShader->setMat4("cpu_Transform", transform);
            }

            if (c_Registry.all_of<MeshComponent>(entity)) {
                auto& mesh = c_Registry.get<MeshComponent>(entity);
                mesh.c_Mesh->Draw(c_LineMode);
            }
        }
    }

    void Scene::RenderLight(const Camera &camera) {
        auto group = c_Registry.group<TagComponent, TransformComponent>(entt::get<>);

        for (auto material : group) {
            if (c_Registry.all_of<MaterialComponent>(material)) {
                std::vector<uint32_t> index(5);
                c_Registry.get<MaterialComponent>(material).ResetLight();
                for (auto light : group) {
                    if (c_Registry.all_of<LightComponent>(light)) {
                        c_Registry.get<MaterialComponent>(material).AddLight(c_Registry.get<LightComponent>(light).Type);
                        auto& shader = c_Registry.get<MaterialComponent>(material).GetShader();
                        auto& position = c_Registry.get<TransformComponent>(light).Position;
                        c_Registry.get<LightComponent>(light).bind(shader, position, index[static_cast<uint32_t>(c_Registry.get<LightComponent>(light).Type)]);
                        index[static_cast<uint32_t>(c_Registry.get<LightComponent>(light).Type)]++;
                    }
                }
            }
        }
    }

    Entity Scene::GetRoot() {
        return { c_Root, this };
    }

    Entity Scene::CreateEntity(const std::string &name, entt::entity parent) {
        if(parent == entt::null) {
            parent = c_Root;
        }
        entt::entity entityIndex = c_Registry.create();

        Entity entity = { entityIndex, this };

        entity.AddComponent<TransformComponent>();
        entity.AddComponent<Relationship>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        auto root = Entity(parent, this);
        root.AddChild(entity);
        return entity;
    }

    Scene::Scene() {
        entt::entity entityIndex = c_Registry.create();
        Entity entity = { entityIndex, this };
        entity.AddComponent<Relationship>();
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = "scene";
        c_Root = entityIndex;

        c_DefaultTexture = Texture2D::create(1, 1);
        uint32_t defaultData = 0xffffffff;
        c_DefaultTexture->setData(&defaultData, sizeof(uint32_t));
        c_DefaultColor = glm::vec4{ 1.0f };

        /// TODO
        ///< Default shader is as same as grid shader.
        c_DefaultShader = Shader::create(__get_abs_path__("/Asset/shader/default.shader"));
    }

    Scene::~Scene() {

    }

}
