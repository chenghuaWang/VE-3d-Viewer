/*!
 *@author   chenghua.wang
 *@file     Violet/rsc/violet_app.cpp
 *@brief    A simple orgnized launcher class.
 */

#include "../include/violet_app.hpp"

#define STB_IMAGE_IMPLEMENTATION

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[Opengl Error] (" << error << "):" << function << " " <<
                  file << ":" << line << std::endl;
        return false;
    }
    return true;
}

namespace violet{

    float App::m_buffer_bar_val = 0;

    App::App(): m_window_on(false), m_progress_window_on(false), m_choose_window_on(false),m_window_handle(nullptr) {
        m_window_on = InitConetx();
        m_progress_window_on = m_window_on;
    };

    App::~App() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window_handle);
        glfwTerminate();
    }

    bool App::InitConetx() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) return false;

        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        m_window_handle = glfwCreateWindow(1202, 676, "Violet Launcher", NULL, NULL);

        if (m_window_handle == nullptr) return false;

        glfwMakeContextCurrent(m_window_handle);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window_handle, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		    std::cout << "Failed to initialize GLAD" << std::endl;
		    return false;
        }

        return true;
    }

    void App::onUpdate() {
        if (m_progress_window_on) {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(0,0), 0, ImVec2(0, 0));

            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            ImGui::SetNextWindowBgAlpha(0);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::Begin("Violet Launcher detection", NULL,    
            ImGuiWindowFlags_NoResize|
            ImGuiWindowFlags_NoMove|
            ImGuiWindowFlags_NoTitleBar|
            ImGuiWindowFlags_NoBringToFrontOnFocus|
            ImGuiWindowFlags_NoInputs|
            ImGuiWindowFlags_NoCollapse|
            ImGuiWindowFlags_NoScrollbar);
            {
                ImVec2 tmp = ImGui::GetContentRegionAvail();
                ImGui::Image(m_bk_image, {tmp.x, tmp.y - 30});
                ImGui::ProgressBar(m_buffer_bar_val, {tmp.x, 30});
            }

            /* The Condition Block under this is just for debug. */
            /* need to change m_buffer_bar_val to the item _check_file_ has already processed! */
            if (m_buffer_bar_val < 1) {
                m_buffer_bar_val += 0.02;
            } else {
                m_progress_window_on = false;
                m_choose_window_on = true;
                std::string tmp_path = "/Asset/ve_config.json";
                if (!_check_file_(_parse_json_(_get_absolute_path_(tmp_path)))) {
                    std::cout << "Can't boot Evergarden, some basic file is missing.\n";
                    exit(1);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
        
        if (m_choose_window_on) {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(0,0), 0, ImVec2(0, 0));

            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            ImGui::SetNextWindowBgAlpha(0);

            ImGui::Begin("Violet Launcher", NULL, ImGuiWindowFlags_NoResize);
            {
                ImGui::Text(HELP_INFO);
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".json", ".");

                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk()) 
                    {
                        m_scene_path = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::cout << "Opening File: " << m_scene_path <<std::endl;
                    }
                    ImGuiFileDialog::Instance()->Close();
                }

            }
            ImGui::End();
        }
    }

    void App::_help_window_() {
        ImGui::Begin("Help", NULL, ImGuiWindowFlags_NoTitleBar);
        {
            ImGui::Text(HELP_INFO);
        }
        ImGui::End();
    }

    void App::run() {
        if (m_window_on) 
        {   
            std::string bk_path_cur("/Asset/image/ve_bkg.jpg");
            std::string bk_image_path = _get_absolute_path_(bk_path_cur);

            int width, height, nrChannels;
            unsigned char *data = stbi_load(bk_image_path.c_str(), &width, &height, &nrChannels, 4);

            if (data == nullptr) {
                std::cerr << "Data from " << bk_image_path << " can't be load \n";
                return;
            }

            GLuint m_textureID;
            GLCall(glGenTextures(1, &m_textureID));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

            stbi_image_free(data);
            m_bk_image = (GLuint *)m_textureID;
            
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            while (!glfwWindowShouldClose(m_window_handle)) {
                glfwPollEvents();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Draw something
                {
                    onUpdate();
                }

                ImGui::Render();

                int display_w, display_h;
                glfwGetFramebufferSize(m_window_handle, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(m_window_handle);
            }
            
        }
    }

    std::vector<std::string> App::_parse_json_(std::string &rhs) {
        std::ifstream f(rhs);

        if (!f.good()) {
            std::cout << "Warn:" << rhs << " Path Err!\n";
        }

        std::string line;
        std::stringstream ss;
        while (std::getline(f, line)) {
            ss << line << "\n";
        }
        std::string err;

        m_json = m_json.parse(ss.str(), err);

        if (!err.empty()) {
            std::cout << "When parse json file. found err \n" << "Err: " << err << std::endl;
        }

        std::vector<std::string> name;
        
        name.push_back(m_json["MagicKey"].string_value());
        name.push_back(m_json[RENDER_JSON_KEY].string_value());
        name.push_back(m_json[EDITOR_JSON_KEY].string_value());
        name.push_back(m_json[CORE_JSON_KEY].string_value());
        return name;
    }

    bool App::_check_file_(std::vector<std::string> rhs) {
        bool res = true;
        std::string magic_key_tmp = rhs[0];
        std::cout << "Found magic code " << magic_key_tmp << std::endl;
        if (rhs[0] != MAGIC_KEY) {
            std::cout << "This config.json not belong to this project.\n";
            return false;
        }
        for (auto &item:rhs){
            if (!_file_exists_(item)) {
                res = false;
                std::cout << "Can't find: " << item << std::endl;
            }
        }
        return res;
    }
        
    bool App::_file_exists_(std::string &rhs) {
        struct stat buffer;   
        return (stat (rhs.c_str(), &buffer) == 0);
    }

    void App::_exec_evergarden_(std::string &rhs) {

    }
        
    void App::_exit_() {
        
    }
        
    std::string App::_join_path_(std::string &lhs, std::string rhs) {
        return lhs + "/" + rhs;
    }

    std::string App::_get_absolute_path_(std::string &rhs) {
        std::string tmp(VE_XSR(VE_ROOT_DIR));
        return tmp + rhs;
    }


    
} // namespace viole
