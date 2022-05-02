#include <iostream>
#include "App/include/Application.hpp"

#include "Editor/include/editor_se_layer.hpp"
#include "Editor/include/editor_scene_layer.hpp"
#include "Editor/include/editor_main_layer.hpp"

using namespace evergarden;

int main() {
    
    App* application = App::create();

    auto* lay_main = new Layer_Main();
    auto *lay_main_cam = new Layer_MainCamera();
    auto* lay_scene = new Layer_Scene(lay_main_cam);
    auto* lay_se = new Layer_ShaderEditor();

    lay_main->setMainCamera_instance(lay_main_cam);
    lay_main->setScene_instacne(lay_scene);
    lay_main->init();

    application->pushLayer(lay_main);
    application->pushLayer(lay_main_cam);
    application->pushLayer(lay_scene);
    application->pushLayer(lay_se);
    application->run();

    delete application;

}