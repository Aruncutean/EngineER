


#define SDL_MAIN_HANDLED
#include <iostream>
#include "MainWindow.hpp"
#include "winodws/ProjectWindow.hpp"


int main(int argc, char* argv[]) {

    MainWindow* mainWindow = new MainWindow;

    mainWindow->AddWindow(std::make_shared<ProjectWindow>(mainWindow));

    mainWindow->Run();

    delete mainWindow;
    return 0;
}