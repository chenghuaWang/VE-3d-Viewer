/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_entity.cpp
 * @brief   A part of entity component system.
 */

#include "../include/render_entity.hpp"

namespace evergarden {

    Entity::Entity(entt::entity handle, Scene *scene):
            c_EntityHandle(handle), c_Scene(scene) {}

    bool Entity::SameOf(Entity &node) {
        entt::entity nodeHandle;
        node.GetHandle(nodeHandle);
        return (nodeHandle == c_EntityHandle);
    }

    bool Entity::HasParent() {
        auto& relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return (c_Scene->c_Registry.valid(relation.parent));
    }

    bool Entity::HasChild() {
        auto& relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return (c_Scene->c_Registry.valid(relation.first));
    }

    bool Entity::IsRoot() {
        return (c_Scene->c_Root == c_EntityHandle);
    }

    Entity Entity::GetFirstChild() {
        if (!c_Scene->c_Registry.valid(c_EntityHandle)) {
            return { entt::null, c_Scene };
        }
        auto relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return { relation.first, c_Scene };
    }

    Entity Entity::GetNext() {
        if (!c_Scene->c_Registry.valid(c_EntityHandle)) {
            return { entt::null, c_Scene };
        }
        auto relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return { relation.next, c_Scene };
    }

    Entity Entity::GetParent() {
        if (!c_Scene->c_Registry.valid(c_EntityHandle)) {
            return { entt::null, c_Scene };
        }
        auto relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return { relation.parent, c_Scene };
    }

    Entity Entity::GetPrev() {
        if (!c_Scene->c_Registry.valid(c_EntityHandle)) {
            return { entt::null, c_Scene };
        }
        auto relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        return { relation.prev, c_Scene };
    }

    void Entity::AddChild(Entity &node) {
        entt::entity nodeHandle;
        node.GetHandle(nodeHandle);

        auto& relation_this = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        auto& relation_node = c_Scene->c_Registry.get<Relationship>(nodeHandle);
        if (c_Scene->c_Registry.valid(relation_this.first)) {
            auto& relation_first = c_Scene->c_Registry.get<Relationship>(relation_this.first);
            relation_first.prev = nodeHandle;
        }
        relation_node.next = relation_this.first;
        relation_node.parent = c_EntityHandle;
        relation_this.first = nodeHandle;
        relation_this.children++;
    }

    void Entity::AddSibling(Entity &node) {
        entt::entity nodeHandle;
        node.GetHandle(nodeHandle);

        auto& relation_this = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        auto& relation_node = c_Scene->c_Registry.get<Relationship>(nodeHandle);
        if (c_Scene->c_Registry.valid(relation_this.next)) {
            auto& relation_next = c_Scene->c_Registry.get<Relationship>(relation_this.next);
            relation_next.prev = nodeHandle;
        }
        relation_node.next = relation_this.next;
        relation_this.next = nodeHandle;
        relation_node.prev = c_EntityHandle;
        relation_node.parent = relation_this.parent;

        if (c_Scene->c_Registry.valid(relation_this.parent)) {
            auto& relation_this_parent = c_Scene->c_Registry.get<Relationship>(relation_this.parent);
            relation_this_parent.children++;
        }
    }

    void Entity::MoveAsChildOf(Entity &dstNode) {
        entt::entity dstNodeHandle;
        dstNode.GetHandle(dstNodeHandle);
        auto& relation_src = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        auto& relation_dst = c_Scene->c_Registry.get<Relationship>(dstNodeHandle);
        auto& relation_src_parent = c_Scene->c_Registry.get<Relationship>(relation_src.parent);

        Entity tmp = dstNode.GetParent();

        if (dstNode.IsSonOf((*this)) || (*this).SameOf(tmp)) return;

        relation_src_parent.children--;
        relation_dst.children++;

        if (c_Scene->c_Registry.valid(relation_src.next)) {
            auto& relation_src_next = c_Scene->c_Registry.get<Relationship>(relation_src.next);
            relation_src_next.prev = relation_src.prev;
        }
        if (c_Scene->c_Registry.valid(relation_src.prev)) {
            auto& relation_src_prev = c_Scene->c_Registry.get<Relationship>(relation_src.prev);
            relation_src_prev.next = relation_src.next;
        }
        else {
            relation_src_parent.first = relation_src.next;
        }
        relation_src.next = relation_dst.first;
        relation_src.prev = entt::null;
        relation_src.parent = dstNodeHandle;

        if (c_Scene->c_Registry.valid(relation_dst.first))
        {
            auto& relation_dst_first = c_Scene->c_Registry.get<Relationship>(relation_dst.first);
            relation_dst_first.prev = c_EntityHandle;
        }
        relation_dst.first = c_EntityHandle;
    }

    void Entity::Remove() {
        auto& relation = c_Scene->c_Registry.get<Relationship>(c_EntityHandle);
        auto& relation_parent = c_Scene->c_Registry.get<Relationship>(relation.parent);
        relation_parent.children--;
        if (c_Scene->c_Registry.valid(relation.prev)) {
            auto& relation_prev = c_Scene->c_Registry.get<Relationship>(relation.prev);
            relation_prev.next = relation.next;
        }
        else {
            relation_parent.first = relation.next;
        }
        Entity child = (*this).GetFirstChild();
        while (child) {
            auto tmp = child.GetNext();
            child.Remove();
            child = tmp;
        }
        c_Scene->c_Registry.destroy(c_EntityHandle);
    }

    Entity Entity::CreateChild(const std::string &name) {
        return c_Scene->CreateEntity(name, c_EntityHandle);
    }

    bool Entity::IsSonOf(Entity &node) {
        if (!node) return false;
        if (node.SameOf(*this)) return true;
        auto child = node.GetFirstChild();
        while (child) {
            if ((*this).SameOf(child)) return true;
            child = child.GetNext();
        }
        return false;
    }

} //! namespace evergarden
