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
    scene.addTriangle(5.0f, Vector3(-2, 0, 0)); // Ajout d'un triangle à la scène pour test
    scene.addQuad(5.0f, 5.0f); // Ajout d'un quad à la scène pour testWritePixel(p, Vector3(0, 0, 1));
    // TEST : vérifie que les objets ont bien une échelle
    for (const Object3D& obj : scene.getObjects()) {
        std::cout << "Object scale: (" << obj.transform.scale.x << ", " 
                << obj.transform.scale.y << ", " 
                << obj.transform.scale.z << ")" << std::endl;
    }
    while(isRunning){
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
        Render();
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
