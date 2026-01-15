#pragma once
#include "Renderer.h"
#include "Event/handleEvent.h"

class App {
    private:
        Window window;
        bool isRunning;
        bool isInitialized;

        
    public:
        App(const std::string& title, int width, int height);
        ~App();

        bool Initialisation();
        void Close();
        void Run();
        void Exit();
        void Update();
};