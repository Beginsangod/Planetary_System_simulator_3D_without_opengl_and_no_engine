#include "core/App.h"
#include <iostream>

int main(int argc,char** argv){
    
    // Création de l'application principale
    App Simulator("Simulateur de Gravité 3D", 1280, 720);
    
    // Initialisation et exécution de l'application
    std::cout<<"Démarrage du simulateur..."<<std::endl;
    Simulator.Initialisation();

    // TEST de multiplication matrice
    Matrice4 testM = Matrice4::Translation(1, 2, 3);
    point testP(Vector3(0, 0, 0));
    point result = testP * testM;
    std::cout << "TEST: point(0,0,0) après translation(1,2,3) = (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;
    // Devrait afficher (1, 2, 3)
    
    // Lancement de la boucle principale
    Simulator.Run();
    
    return 0;
}
// Le Destructeur de l'application principale 
// sera appelé automatiquement pas besoin de fonction close...