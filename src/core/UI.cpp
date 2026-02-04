#include "UI.h"
#include "../Simulation/SimulationManager.h"
#include "../physics/world.h"
#include <cstring>
#include <iostream>

UI::UI(SDL_Renderer* renderer, SDL_Window* window, SimulationManager* sm, World* w) 
    : renderer(renderer)
    , window(window)
    , simManager(sm)
    , world(w)
    , showKeyboardShortcutsMenu(false)
    , showSceneManagerMenu(false)
    , windowWidth(800)
    , windowHeight(600)
    , showAddScenePopup(false)
    , selectedSceneIndex(0)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Style sombre par défaut
    ImGui::StyleColorsDark();
    
    // Personnalisation du style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    
    // Couleurs personnalisées pour le thème spatial
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.15f, 0.95f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.25f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.35f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.35f, 0.65f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.45f, 0.75f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.55f, 0.85f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.3f, 0.4f, 0.7f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.5f, 0.8f, 0.8f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.6f, 0.9f, 0.8f);

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    
    // Initialiser le buffer du nom de scène
    memset(NewSceneName, 0, sizeof(NewSceneName));
    
    InitializeShortcuts();
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
}

UI::~UI(){
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void UI::InitializeShortcuts() {
    shortcuts.clear();
    
    // Raccourcis de caméra
    shortcuts.push_back({"Z/W", "Avancer", "Caméra"});
    shortcuts.push_back({"S", "Reculer", "Caméra"});
    shortcuts.push_back({"Q/A", "Déplacement gauche", "Caméra"});
    shortcuts.push_back({"D", "Déplacement droite", "Caméra"});
    shortcuts.push_back({"Flèche Haut", "Monter (mode vol libre)", "Caméra"});
    shortcuts.push_back({"Flèche Bas", "Descendre (mode vol libre)", "Caméra"});
    shortcuts.push_back({"Shift Gauche", "Sprint (vitesse x2)", "Caméra"});
    shortcuts.push_back({"Ctrl Gauche", "Vitesse rapide (10.0)", "Caméra"});
    shortcuts.push_back({"Clic droit + Souris", "Rotation de la caméra", "Caméra"});
    shortcuts.push_back({"C", "Basculer mode caméra (Vol/Marche)", "Caméra"});
    
    // Raccourcis de rendu
    shortcuts.push_back({"Tab", "Changer mode de rendu", "Rendu"});
    shortcuts.push_back({"L", "Activer/Désactiver éclairage", "Rendu"});
    
    // Raccourcis généraux
    shortcuts.push_back({"F1", "Afficher/Masquer raccourcis clavier", "Général"});
    shortcuts.push_back({"F2", "Afficher/Masquer gestionnaire de scènes", "Général"});
}

void UI::RenderKeyboardShortcutsMenu() {
    if (!showKeyboardShortcutsMenu) return;
 
    ImGui::SetNextWindowPos(ImVec2(windowWidth - 410, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 550), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin(" Raccourcis Clavier", &showKeyboardShortcutsMenu, ImGuiWindowFlags_NoResize)) {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Guide des raccourcis");
        ImGui::Separator();
        ImGui::Spacing();
        
        // Organiser par catégorie
        std::string currentCategory = "";
        
        for (const auto& shortcut : shortcuts) {
            if (shortcut.category != currentCategory) {
                currentCategory = shortcut.category;
                
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
                ImGui::Text("---- %s ----", currentCategory.c_str());
                ImGui::PopStyleColor();
                ImGui::Spacing();
            }
            
            // Afficher la touche et sa description
            ImGui::BulletText("%s", shortcut.key.c_str());
            ImGui::SameLine(180);  // Alignement
            ImGui::TextWrapped("%s", shortcut.description.c_str());
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Appuyez sur F1 pour fermer ce menu");
    }
    ImGui::End();
}

void UI::RenderSceneManagerMenu() {
    if (!showSceneManagerMenu) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin(" Gestionnaire de Scènes", &showSceneManagerMenu)) {
        // Afficher la scène actuelle
        if (simManager) {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Scène actuelle:");
            ImGui::SameLine();
            ImGui::Text("%s", simManager->GetCurrentSimulation().c_str());
            
            // Bouton de rafraîchissement
            ImGui::SameLine();
            if (ImGui::SmallButton("Actualise")) {
                simManager->RefreshSceneList(*world);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Rafraîchir la liste des scènes");
            }
        }
        
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::BeginChild("SceneList", ImVec2(0, -50), true);
        
        if (simManager) {
            std::vector<std::string> sceneNames = simManager->GetSceneNames();
            std::string currentScene = simManager->GetCurrentSimulation();
            
            for (int i = 0; i < sceneNames.size(); i++) {
                bool isCurrent = (sceneNames[i] == currentScene);
                std::string displayName = isCurrent ? "£ " + sceneNames[i] : "  " + sceneNames[i];
                
                if (ImGui::Selectable(displayName.c_str(), selectedSceneIndex == i)) {
                    selectedSceneIndex = i;
                }
                
                // Double-clic pour charger
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    simManager->LoadSimulation(*world, sceneNames[i]);
                    std::cout << " Chargement: " << sceneNames[i] << std::endl;
                }
                
                // Menu contextuel
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem(" Charger")) {
                        simManager->LoadSimulation(*world, sceneNames[i]);
                    }
                    
                    if (ImGui::MenuItem(" Sauvegarder", nullptr, false, isCurrent)) {
                        simManager->SaveCurrentSimulation();
                    }
                    
                    if (ImGui::MenuItem(" Supprimer", nullptr, false, !isCurrent)) {
                        simManager->DeleteSimulation(sceneNames[i]);
                    }
                    
                    ImGui::EndPopup();
                }
            }
        }
        
        ImGui::EndChild();
        
        // Bouton d'ajout
        if (ImGui::Button("+ Ajouter une nouvelle scène", ImVec2(-1, 35))) {
            showAddScenePopup = true;
            memset(NewSceneName, 0, sizeof(NewSceneName));
        }
    }
    ImGui::End();
    
    RenderAddScenePopup();
}

void UI::RenderAddScenePopup() {
    if (!showAddScenePopup) return;
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.5f, windowHeight * 0.5f),  ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(" Créer une nouvelle scène", &showAddScenePopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Entrez le nom de la nouvelle scène :");
        ImGui::Spacing();
        ImGui::SetNextItemWidth(350);
        bool enterPressed = ImGui::InputText("##SceneName", NewSceneName, sizeof(NewSceneName), ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Spacing();

        //  Vérifier si le nom existe déjà
        bool nameExists = false;
        if (simManager && strlen(NewSceneName) > 0) {
            nameExists = simManager->SceneExists(NewSceneName);
            if (nameExists) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " Ce nom existe déjà!");
            }
        }
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), " Un fichier .wrld sera créé dans 'world'");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Désactiver le bouton si le nom existe
        if (nameExists) {
            ImGui::BeginDisabled();
        }
        
        if (ImGui::Button(" Créer", ImVec2(170, 0)) || (enterPressed && !nameExists)) {
            if (strlen(NewSceneName) > 0 && simManager) {
                if (simManager->CreateNewSimulation(NewSceneName)) {
                    std::cout << " Scène créée avec succès: " << NewSceneName << std::endl;
                    showAddScenePopup = false;
                }
            }
        }
        
        if (nameExists) {
            ImGui::EndDisabled();
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button(" Annuler", ImVec2(170, 0))) {
            showAddScenePopup = false;
        }
        
        ImGui::EndPopup();
    }
    
    if (showAddScenePopup && !ImGui::IsPopupOpen(" Créer une nouvelle scène")) {
        ImGui::OpenPopup(" Créer une nouvelle scène");
    }
}

void UI::UpdateUI() {
    // Mettre à jour les dimensions de fenêtre
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    // Afficher les menus
    RenderKeyboardShortcutsMenu();
    RenderSceneManagerMenu();
    
}