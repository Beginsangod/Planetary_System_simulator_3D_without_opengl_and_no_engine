#pragma once
#include "Renderer.h"
#include "Event/handleEvent.h"
#include "../graphics/Renderer3d.h"
#include "../physics/MotorPhysics.h"
#include "../Simulation/SimulationManager.h"
#include "UI.h"

//classe application principale
class App {
    private:
        Window window;
        bool isRunning;
        bool isInitialized;
        World mworld;
        SimulationManager* SM = nullptr; // Gère les Scènes
        MotorPhysics* motorPhysics = nullptr;
        Renderer3d* renderer3d = nullptr;
        UI* ui = nullptr;
        
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