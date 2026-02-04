#include "world.h"

void World::addStar(const Vector3& position, float mass, float radius, float luminosity, const Vector3& color, float temperature, const Vector3& velocity) {
    addObject(std::make_unique<Star>(position, mass, radius, luminosity, color, temperature, velocity));
}

void World::addPlanet(const Vector3& position, float mass, float radius, const Vector3& color, const Vector3& rotationSpeed, float AxialTilt, const Vector3& velocity) {
    addObject(std::make_unique<planete>(position, mass, radius, rotationSpeed, AxialTilt, color, velocity));
}

void World::addSatellite(const Vector3& position, float mass, float radius, const Vector3& color, const Vector3& velocity) {
    addObject(std::make_unique<Satellite>(position, mass, radius, color, velocity));
}
