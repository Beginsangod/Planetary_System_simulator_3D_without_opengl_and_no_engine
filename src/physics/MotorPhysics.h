
#pragma once
#include "world.h"

class MotorPhysics {
    private:
        const float g = 6.674e-3f; //  
        const float epsoftening = 0.01f;// Constante d'adoucissement pour éviter les Boums Numériques

        // Elimination des collisions
        void EliminateCollisions(std::vector<Astre*>& astres);

    public:
        World& world;

        MotorPhysics(World& world) : world(world) {};
        void Update(float deltaTime);
};