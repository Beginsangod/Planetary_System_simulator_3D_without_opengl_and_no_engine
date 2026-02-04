#pragma once
#include "../graphics/scene.h"
#include "Astres.h"

class World : public Scene{
    public:
        
        void addStar(const Vector3& position, float mass, float radius, float luminosity, const Vector3& color, float temperature, const Vector3& velocity);
        void addPlanet(const Vector3& position, float mass, float radius, const Vector3& color, const Vector3& rotationSpeed, float AxialTilt, const Vector3& velocity);
        void addSatellite(const Vector3& position, float mass, float radius, const Vector3& color, const Vector3& velocity);
};
