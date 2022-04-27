#include "../include/violet_app.hpp" 

int main() {
    violet::App *App_ptr = violet::CreateApp();
    App_ptr->run();
    delete App_ptr;
}