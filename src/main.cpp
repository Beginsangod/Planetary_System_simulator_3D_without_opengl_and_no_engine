#include "core/App.h"
#include <iostream>

int main(int argc,char** argv){
    
    App Simulator("Simulateur de Gravité 3D", 800, 600);
    
    std::cout<<"Démarrage du simulateur..."<<std::endl;
    Simulator.Initialisation();
    
    Simulator.Run();

    Simulator.Exit();
    
    return 0;
}