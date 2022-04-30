/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_se_widget.cpp
 * @brief   Shader editore widget.
 */

#include "../include/editor_se_widget.hpp"

namespace evergarden {

    void Widget_ShaderEditor::init() {

        auto lang = TextEditor::LanguageDefinition::GLSL();
        m_cursor_pos = m_editor.GetCursorPosition();
        // Basic Editor Language show setting Modify
        static const char* identifiers[] = {
            "EmissionColor",
            "Intensity",
            "Material",
            "UseNormalMap",
            "HeightScale",
            "diffuse",
            "specular",
            "color",
            "colour"
        };
        static const char* idecls[] = {
                "u_EmissionColor",
                "u_Intensity",
                "u_Material",
                "u_UseNormalMap",
                "u_HeightScale",
                "diffuse",
                "specular",
                "color",
                "colour"
        };
        for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i) {
            TextEditor::Identifier id;
            id.mDeclaration = std::string(idecls[i]);
            lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
        }
        m_editor.SetLanguageDefinition(lang);
    }

    void Widget_ShaderEditor::onGuiRender() {

        ImGui::Text("Shader Editor");
            ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", m_cursor_pos.mLine + 1, m_cursor_pos.mColumn + 1, m_editor.GetTotalLines(),
                        m_editor.IsOverwrite() ? "Ovr" : "Ins",
                        m_editor.CanUndo() ? "*" : " ",
                        m_editor.GetLanguageDefinition().mName.c_str(), "Just For test.");

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save File", "Ctrl-S"))
                    {
                        //TODO  
                    }
                    if (ImGui::MenuItem("Open File", "Ctrl-O")) 
                    {
                        //TODO  
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    bool ro = m_editor.IsReadOnly();
                    if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                        m_editor.SetReadOnly(ro);
                    ImGui::Separator();

                    if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
                        m_editor.Undo();
                    if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
                        m_editor.Redo();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
                        m_editor.Copy();
                    if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_editor.HasSelection()))
                        m_editor.Cut();
                    if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
                        m_editor.Delete();
                    if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                        m_editor.Paste();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Select all", "Ctrl-A", nullptr))
                        m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Dark palette"))
                        m_editor.SetPalette(TextEditor::GetDarkPalette());
                    if (ImGui::MenuItem("Light palette"))
                        m_editor.SetPalette(TextEditor::GetLightPalette());
                    if (ImGui::MenuItem("Retro blue palette"))
                        m_editor.SetPalette(TextEditor::GetRetroBluePalette());
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Compile and Link")) {
                    if (ImGui::MenuItem("Compile Test")){
                        // TODO
                    }
                    if (ImGui::MenuItem("Compile to ShaderZoo")) {
                        // TODO
                        // TODO Use ImGUI::methods to Point error to lines
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            m_editor.Render("TextEditor");
    }

}