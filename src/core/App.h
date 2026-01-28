#pragma once
#include "Renderer.h"
#include "Event/handleEvent.h"
#include "../graphics/Renderer3d.h"

//classe application principale
class App {
    private:
        Window window;
        bool isRunning;
        bool isInitialized;
        Scene scene;
        Renderer3d* renderer3d = nullptr;
        
    public:
        App(const std::string& title, int width, int height);
        ~App();

        bool Initialisation();
        void Close();
        void Run();
        void Exit();
        void Render();
        void HandleCameraInput(float deltaTime);
        void HandleInputCommand();
};