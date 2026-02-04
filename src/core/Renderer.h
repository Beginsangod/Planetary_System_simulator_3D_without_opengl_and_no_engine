#pragma once
#include <SDl3/SDL.h>
#include <string>

using namespace std;

//classe fenetre
class Window {
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        std::string wtitle;
        int wwidth;
        int wheight;
        bool isInitiase;

    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool Initialisation();
        void CloseWindow();
        void ClearWindow();
        void Present();
        void ExitError(const std::string& message);

        //Getters
        SDL_Renderer* GetSDLRenderer() const { return renderer; }
        SDL_Window* GetSDLWindow() const { return window; }
        int GetWidth() const { return wwidth; }
        int GetHeight() const { return wheight; }
};