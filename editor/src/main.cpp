


#define SDL_MAIN_HANDLED
#include <iostream>
#include "MainWindow.hpp"
#include "windows/ProjectView.hpp"
#include "windows/ProjectWindow.hpp"


int main(int argc, char* argv[]) {

    MainWindow* mainWindow = new MainWindow;

    mainWindow->AddWindow(std::make_shared<ProjectWindow>(mainWindow));

    service_editor::ViewService::Instance().AddView(std::make_shared<ProjectView>());

    mainWindow->Run();

    delete mainWindow;
    return 0;
}