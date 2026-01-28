#include "core/App.h"
#include <iostream>

int main(int argc, char** argv){
    
    // Création de l'application principale
    App Simulator("Simulateur de Gravité 3D", 1280, 720);
    
    // Initialisation et exécution de l'application
    std::cout<<"Démarrage du simulateur..."<<std::endl;
    Simulator.Initialisation();

    // Lancement de la boucle principale
    Simulator.Run();
    
    return 0;
}
// Le Destructeur de l'application principale 
// sera appelé automatiquement pas besoin de fonction close...