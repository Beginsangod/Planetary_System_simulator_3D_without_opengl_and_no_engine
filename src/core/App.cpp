#include <iostream>
#include "App.h"

App::App(const std::string& title, int width, int height) :
    window(title, width, height),
    isRunning(false),
    isInitialized(false) {};

App::~App(){
    Close();
}

bool App::Initialisation(){
    if (!window.Initialisation())
    {
        return false;
    }

    isInitialized = true;
    std::cout<< "initialisation du simulateur terminé"<<std::endl;
    return true;
}

void App::Close(){
    window.CloseWindow();
    std::cout<<"fermeture du simulateur"<<std::endl;
}

void App::Run(){
    if (!isInitialized)
    {
        return;
    }
    
    isRunning = true;

    while(isRunning){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }

        window.Present();
    }
}

void App::Exit(){
    isRunning = true;
    std::cout << "🔄 Simulateur arrêté" << std::endl;
}
