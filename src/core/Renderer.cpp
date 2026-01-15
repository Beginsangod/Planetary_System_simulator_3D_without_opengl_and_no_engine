#include "Renderer.h"
#include <iostream>

//constructeur fenetre
Window::Window(const std::string& title, int width, int height)
    : wtitle(title),
      wwidth(width),
      wheight(height) {};

//Destructeur fenetre
Window::~Window(){
    CloseWindow();
}

//Initialisation fenetre
bool Window::Initialisation(){
    std::cout << "SDL revision: " << SDL_GetRevision() << std::endl;
    if(!SDL_Init(SDL_INIT_VIDEO))
        {ExitError("Initialisation SDL3"); return false;}
    
    if(!SDL_CreateWindowAndRenderer(wtitle.c_str(), wwidth, wheight, SDL_WINDOW_RESIZABLE, &window, &renderer))
        {ExitError("Windows or renderer"); return false;}

    isInitiase = true;
    std::cout<< "✔ Fenêtre initialisée avec succès " <<std::endl;
    return true;
};

//Fermeture fenetre 
void Window::CloseWindow(){
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    SDL_Quit();
    isInitiase = false;
    std::cout << "fenetre fermée avec succès"<< std::endl;
}

//nettoyage ecran
void Window::ClearWindow(){
    if (renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
}

//Affichage
void Window::Present() {
    if (renderer) {
        SDL_RenderPresent(renderer);
    }
}

//Fonction Pour redaction message d'erreur et quit de SDL en sécurité
void Window::ExitError(const std::string& message){
    std::cout<<"Erreur : " << message.c_str() << " : " << SDL_GetError() <<std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
}  

