#include "../include/editor_file_widget.hpp"

namespace evergarden {
    // static std::string path_got = "";

    void GuiFileWidget::_on_open_window_(const std::string &_describe_, const std::string &_class_) {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", _describe_.c_str(), _class_.c_str(), ".");
    }

    void GuiFileWidget::_on_window_react_(std::function<void(const std::string &)> func) {
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                // path_got = ImGuiFileDialog::Instance()->GetFilePathName();
                // c_opened_file_path = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                // func(path_got);
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }

}