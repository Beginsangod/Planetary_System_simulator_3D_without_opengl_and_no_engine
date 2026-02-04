#pragma once
#include "../../ThirdParty/imgui.h"
#include "../../ThirdParty/backends/imgui_impl_sdl3.h"
#include "../../ThirdParty/backends/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>

// Permet d'éviter les dépendances circulaires
class SimulationManager;
class World;

// Structure pour stocker les informations d'un raccourci clavier
struct KeyboardShortcut {
    std::string key;          
    std::string description;   
    std::string category;  
};

class UI {
    private:
        SDL_Renderer* renderer;   
        SDL_Window* window; 
        SimulationManager* simManager;
        World* world;
        bool showKeyboardShortcutsMenu;
        bool showSceneManagerMenu;
        int windowWidth;
        int windowHeight;
        std::vector<KeyboardShortcut> shortcuts;
        char NewSceneName[128];
        bool showAddScenePopup;
        int selectedSceneIndex;
        
        // Initialiser la liste des raccourcis
        void InitializeShortcuts();
        
        // Rendu des menus individuels
        void RenderKeyboardShortcutsMenu();
        void RenderSceneManagerMenu();
        void RenderAddScenePopup();

    public:
        UI(SDL_Renderer* renderer, SDL_Window* window, SimulationManager* sm, World* w);
        ~UI();

        // Mise à jour des dimensions de fenêtre
        void UpdateWindowSize(int width, int height) { 
            windowWidth = width; 
            windowHeight = height; 
        }
        
        // Basculer l'affichage des menus
        void ToggleKeyboardShortcutsMenu() { showKeyboardShortcutsMenu = !showKeyboardShortcutsMenu; }
        void ToggleSceneManagerMenu() { showSceneManagerMenu = !showSceneManagerMenu; }
        
        // Getters pour l'état des menus
        bool IsKeyboardShortcutsMenuVisible() const { return showKeyboardShortcutsMenu; }
        bool IsSceneManagerMenuVisible() const { return showSceneManagerMenu; }
        
        // Méthode principale de rendu UI
        void UpdateUI();
        void Render() { 
            ImGui::Render(); 
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        };
        
        void ProcessEvent(SDL_Event* event) {
            ImGui_ImplSDL3_ProcessEvent(event);
        }

        void NewFrame() {
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }
};