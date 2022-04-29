#include <iostream>
#include "App/include/Application.hpp"

#include "Editor/include/editor_se_layer.hpp"

using namespace evergarden;

int main() {
    
    App* application = App::create();

    application->pushLayer(new Layer_ShaderEditor());
    application->run();

    delete application;

}