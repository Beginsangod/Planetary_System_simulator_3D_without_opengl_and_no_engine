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
    std::cout<< "initialisation du moteur3D...."<<std::endl;
    renderer3d = new Renderer3d(window.GetWidth(), window.GetHeight(), window.GetSDLRenderer());
    renderer3d->InitializeBuffers();
    std::cout<< "initialisation du simulateur terminé"<<std::endl;
    return true;
}

//fermeture application principale
void App::Close(){
    window.CloseWindow();
    std::cout<<"fermeture du simulateur"<<std::endl;
}

// boucle principale
void App::Run(){
    if (!isInitialized)
    {
        return;
    }
    
    isRunning = true;
    std::cout << "🚀 Simulateur démarré" << std::endl;
    scene.SetCameraAspectRatio(static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()));
    Camera& camera = scene.GetCamera();
    camera.SetMode(CameraMode::GROUND_WALK); // Mode par défaut
    scene.addSphere(1.0f, 32, 64, Vector3(0, 0, 0)); // Ajout d'une sphère à la scène pour test
    Uint64 lastTime = SDL_GetTicks();
    while(isRunning){
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Convertir en secondes
        lastTime = currentTime;

        Keyboard::Update();
        Mouse::Update();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    Exit();
                    break;
                default:
                    break;
            }
        }
        // Gérer les inputs de la caméra
        HandleCameraInput(deltaTime);  
        
        // Inputs du moteur
        HandleInputCommand();

        Render();
    }
}

// NOUVELLE MÉTHODE : Gérer les inputs de la caméra
void App::HandleCameraInput(float deltaTime) {
    Camera& camera = scene.GetCamera();
    
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
        Camera& camera = scene.GetCamera();
        camera.ToggleMode();
        
        if (camera.GetMode() == CameraMode::FREE_FLY) {
            std::cout << "🚁 Mode caméra: VOL LIBRE" << std::endl;
        } else {
            std::cout << "🚶 Mode caméra: MARCHE AU SOL" << std::endl;
        }
    }
}

// rendu de la scène
void App::Render(){
    window.ClearWindow();
    renderer3d->Clear(Vector3(0.1f, 0.1f, 0.1f)); // Couleur de fond gris foncé
    renderer3d->RenderScene(scene);
    renderer3d->Present();
    //window.Present();
}

//sortie de la boucle principale
void App::Exit(){
    isRunning = false;
    std::cout << "🔄 Simulateur arrêté" << std::endl;
}
