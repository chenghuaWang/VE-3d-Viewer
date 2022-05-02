/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/src/editor_hia.cpp
 * @brief   Main Layer contain hia layers.
 */

#include "../include/editor_hia.hpp"
#include "../include/editor_main_layer.hpp"

namespace evergarden {
    void Hia::__on_show_hia__(bool *is_open, Layer_Scene *scene) {
        ImGui::Begin("Component show", is_open);
        {
            auto _scene_ = scene->GetScene();
            if (!_scene_->Empty()){
                auto _node_root_ = _scene_->GetRoot();
                while(_node_root_) {
                    __on__tree__(_node_root_);
                    _node_root_ = _node_root_.GetNext();
                }
            }
        }
        ImGui::End();
    }

    void Hia::__on__tree__(Entity node) {
        if (!node) return;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
        bool open = ImGui::TreeNodeEx(std::to_string(node.ID()).c_str(),
                                      ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen
                                      | (node.SameOf(MainEditorWidget::GetSelectEntity()) ? ImGuiTreeNodeFlags_Selected : 0) | (node.GetFirstChild() ? 0 : ImGuiTreeNodeFlags_Leaf),
                                      "%s", node.GetComponent<TagComponent>().Tag.c_str());
        ImGui::PopStyleVar();

        ImGui::PushID(std::to_string(node.ID()).c_str());
        if (ImGui::BeginPopupContextItem())
        {
            ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());

            MainEditorWidget::AddObjectMenu(node);
            ImGui::Separator();

            if (!node.IsRoot())
            {
                if (ImGui::MenuItem("Delete"))
                    __on_remove__(node);
            }
            //	rename
            if (node)
            {
                ImGui::Separator();
                MainEditorWidget::Rename(node);
            }
            ImGui::EndPopup();
        }
        ImGui::PopID();

        //	left click check
        if (ImGui::IsItemClicked()) MainEditorWidget::SetSelectEntity(node);
        //	Drag&Drop Effect
        MainEditorWidget::SetDragDropSource<Entity>("Hierarchy Drag&Drop", [&](){
            ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());
        }, MainEditorWidget::GetSelectEntity());

        MainEditorWidget::SetDragDropTarget<Entity>("Hierarchy Drag&Drop", [&](Entity srcNode) {
            srcNode.MoveAsChildOf(node);
            auto& transform = srcNode.GetComponent<TransformComponent>();
            transform.SetRelatePosition(srcNode.GetParent().GetComponent<TransformComponent>().Position);
        });
        //	Recurse
        if (open)
        {
            if (node)
            {
                auto child = node.GetFirstChild();
                while (child)
                {
                    __on__tree__(child);
                    child = child.GetNext();
                }
            }
            ImGui::TreePop();
        }

    }

    void Hia::__on_remove__(Entity &node_noneed) {
        auto tmp = node_noneed;
        node_noneed = node_noneed.GetNext();
        tmp.Remove();
        ImGui::CloseCurrentPopup();
    }
}
