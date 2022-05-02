/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_scene.hpp
 * @brief   A API package for scene class.
 */

#ifndef H_RENDER_SCENE
#define H_RENDER_SCENE

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_light.hpp"
#include "render_material.hpp"
#include "render_model.hpp"
#include "render_skybox.hpp"
#include "render_shader.hpp"

#include "render_component.hpp"
#include "render_entity.hpp"

namespace evergarden {

    class Entity;

    struct Relationship {
        size_t children{};
        entt::entity first{ entt::null };
        entt::entity prev{ entt::null };
        entt::entity next{ entt::null };
        entt::entity parent{ entt::null };
    };


    class Scene {
    public:
        friend class Entity;
    public:
        Scene();
        ~Scene();

    public:
        void RenderObject(Camera& camera, const std::shared_ptr<SkyBox>& envMap);
        void RenderLight(const Camera& camera);

        Entity GetRoot();
        bool Empty() { return (c_Root == entt::null); }
        void SetLineMode(const bool& enable) { c_LineMode = enable; }
        bool GetLineMode() const { return c_LineMode; }

    public:
        Entity CreateEntity(const std::string& name = std::string(), entt::entity parent = entt::null);

    private:
        glm::vec4 c_DefaultColor{};
        bool c_LineMode = true;
        std::shared_ptr<Texture2D> c_DefaultTexture;
        std::shared_ptr<Shader> c_DefaultShader;
        entt::registry c_Registry;
        entt::entity c_Root = entt::null;
    };

} //! namespace evergarden

namespace evergarden {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return c_Scene->c_Registry.emplace<T>(c_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        bool HasComponent() {
            return c_Scene->c_Registry.all_of<T>(c_EntityHandle);
        }

        template<typename T>
        void RemoveComponent() {
            c_Scene->c_Registry.remove<T>(c_EntityHandle);
        }

        template<typename T>
        T& GetComponent() {
            return c_Scene->c_Registry.get<T>(c_EntityHandle);
        }

        operator bool() const {
            if (c_Scene == nullptr) return false;
            return c_Scene->c_Registry.valid(c_EntityHandle);
        }

        uint32_t ID() { return static_cast<uint32_t>(c_EntityHandle); }

        void GetHandle(entt::entity& node) { node = c_EntityHandle; }
        bool SameOf(Entity& node);
        bool HasParent();
        bool HasChild();
        bool IsRoot();

        Entity GetFirstChild();
        Entity GetNext();
        Entity GetParent();
        Entity GetPrev();
        void AddChild(Entity& node);
        void AddSibling(Entity& node);
        void MoveAsChildOf(Entity& dstNode);
        void Remove();
        Entity CreateChild(const std::string& name = std::string());

    public:
        Scene* c_Scene = nullptr;

    private:
        bool IsSonOf(Entity& node);

    private:
        entt::entity c_EntityHandle{ entt::null };

    };


}

#endif //! H_RENDER_SCENE
