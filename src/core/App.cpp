#include <iostream>
#include "App.h"

//constructeur application principale
App::App(const std::string& title, int width, int height) :
    window(title, width, height),
    isRunning(false),
    isInitialized(false) {};

// destructeur application principale
App::~App(){
    Close();
}

//initialisation application principale
bool App::Initialisation(){
    if (!window.Initialisation())
    {
        return false;
    }

    isInitialized = true;
    std::cout << "Initialisation du manager de Scène" <<std::endl;
    SM = new SimulationManager(mworld);
    std::cout<< "Initialisation du moteur3D...."<<std::endl;
    renderer3d = new Renderer3d(window.GetWidth(), window.GetHeight(), window.GetSDLRenderer());
    renderer3d->InitializeBuffers();
    std::cout<< "Initialisation du moteur de physique..." <<std::endl;
    motorPhysics = new MotorPhysics(mworld);
    std::cout<<"Initialisation du User Interface..." <<std::endl;
    ui = new UI(window.GetSDLRenderer(), window.GetSDLWindow(), SM, &mworld);
    ui->UpdateWindowSize(window.GetWidth(), window.GetHeight());
    std::cout<< "Initialisation du simulateur terminé"<<std::endl;
    return true;
}

//fermeture application principale
void App::Close(){
    if (ui) {
        delete ui;
        ui = nullptr;
    }
    if (renderer3d) {
        delete renderer3d;
        renderer3d = nullptr;
    } 
    if (motorPhysics) {
        delete motorPhysics;
        motorPhysics = nullptr;
    }
    if (SM) {
        delete SM;
        SM = nullptr;
    }
    window.CloseWindow();
    std::cout<<"🚩 Fermeture du simulateur"<<std::endl;
}

// boucle principale
void App::Run(){
    if (!isInitialized)
    {
        return;
    }
    
    isRunning = true;
    std::cout << "🚀 Simulateur démarré" << std::endl;
    mworld.SetCameraAspectRatio(static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()));
    Camera& camera = mworld.GetCamera();
    camera.SetMode(CameraMode::FREE_FLY);
    Uint64 lastTime = SDL_GetTicks();
    while(isRunning){
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Convertir en secondes
        lastTime = currentTime;

        Keyboard::Update();
        Mouse::Update();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            ui->ProcessEvent(&event);

            switch (event.type) {
                case SDL_EVENT_QUIT:
                    Exit();
                    break;
                default:
                    break;
            }
        }

        // Calcul du moteur physique
        motorPhysics->Update(deltaTime);

        // Gérer les inputs de la caméra
        HandleCameraInput(deltaTime);  
        
        // Inputs du moteur
        HandleInputCommand();

        ui->NewFrame();
        ui->UpdateUI();

        Render();
    }
}

// Gérer les inputs de la caméra
void App::HandleCameraInput(float deltaTime) {
    Camera& camera = mworld.GetCamera();
    
    // Traiter le clavier (déplacement)
    camera.ProcessKeyboard(deltaTime);
    
    // Traiter la souris (rotation) SEULEMENT si bouton droit maintenu
    if (Mouse::IsButtonDown(MouseButton::Right)) {
        Vector2 mouseDelta = Mouse::GetDelta();
        camera.ProcessMouse(mouseDelta.x, mouseDelta.y);
    }
    // Ajuster la vitesse de déplacement avec Ctrl
    if (Keyboard::IsKeyDown(KeyCode::LeftCtrl)) {
        camera.SetMoveSpeed(10.0f); 
    } else {
        camera.SetMoveSpeed(5.0f);   
    }

}

void App::HandleInputCommand(){
    if (Keyboard::IsKeyPressed(KeyCode::Tab)) {
        RenderMode currentMode = renderer3d->GetRenderMode();
        switch (currentMode) {
            case RenderMode::FILLED:
                renderer3d->SetRenderMode(RenderMode::WIREFRAME);
                break;
            case RenderMode::WIREFRAME:
                renderer3d->SetRenderMode(RenderMode::POINTS);
                break;
            case RenderMode::POINTS:
                renderer3d->SetRenderMode(RenderMode::FILLED);
                break;
        }
    }

    // Basculer le mode caméra avec C
    if (Keyboard::IsKeyPressed(KeyCode::C)) {
        Camera& camera = mworld.GetCamera();
        camera.ToggleMode();
        
        if (camera.GetMode() == CameraMode::FREE_FLY) {
            std::cout << "🚁 Mode caméra: VOL LIBRE" << std::endl;
        } else {
            std::cout << "🚶 Mode caméra: MARCHE AU SOL" << std::endl;
        }
    }

    // Désactiver/Activer le lighting
    if (Keyboard::IsKeyPressed(KeyCode::L))
    {
        bool currentState = renderer3d->IsDynamicLightingEnabled();
        renderer3d->SetDynamicLighting(!currentState);
        std::cout << "💡 Éclairage dynamique " << (renderer3d->IsDynamicLightingEnabled() ? "activé" : "désactivé") << std::endl;
    }

    if (Keyboard::IsKeyPressed(KeyCode::F1)) {
        ui->ToggleKeyboardShortcutsMenu();
        std::cout << " Menu raccourcis: " << (ui->IsKeyboardShortcutsMenuVisible() ? "Ouvert" : "Fermé") << std::endl;
    }
    if (Keyboard::IsKeyPressed(KeyCode::F2)) {
        ui->ToggleSceneManagerMenu();
        std::cout << " Menu scènes: " << (ui->IsSceneManagerMenuVisible() ? "Ouvert" : "Fermé") << std::endl;
    }
}

// rendu de la scène
void App::Render(){
    window.ClearWindow();
    renderer3d->Clear(Vector3(0.1f, 0.1f, 0.1f)); // Couleur de fond gris foncé
    renderer3d->RenderScene(mworld);
    renderer3d->Present();
    ui->Render();
    window.Present();
}

//sortie de la boucle principale
void App::Exit(){
    isRunning = false;
    std::cout << "🔄 Simulateur arrêté" << std::endl;
}
