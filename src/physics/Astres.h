
#pragma once
#include "../graphics/Object3D.h"

class Astre: public Object3D {
    public: 
        float mass;
        float radius;
        Vector3 velocity = Vector3::Zero;
        Vector3 acceleration;

        Astre(const Vector3& position , float mass, float radius, const Vector3& color, const Vector3& initialVelocity) : Object3D(position), mass(mass), radius(radius), velocity(initialVelocity) {
            mesh = MeshBuilder::CreateSphere(radius, 32, 64, color);
            material = Material(color);
        }
};

class Star: public Astre {
    public:
        float luminosity; // Intensité lumineuse
        float temperature; // Influe sur la couleur de la lumiere

        Star(const Vector3& position, float mass, float radius, float lm, Vector3 color= Vector3(1.0f, 0.0f, 1.0f), float temperature = 5000, const Vector3& initialVelocity = Vector3::Zero)
            : Astre(position, mass, radius, color, initialVelocity), luminosity(lm), temperature(temperature)
        {
            // Définir le matériau émissif basé sur la luminosité et la couleur
            Material emissiveMaterial = Material::EmissiveWhite(luminosity);
            emissiveMaterial.emissiveColor = color;
            emissiveMaterial.roughness = 0.5;
            SetMaterial(emissiveMaterial);
        }    
        
        virtual std::unique_ptr<Object3D> clone() const {
            return std::make_unique<Star>(*this);
        }
        
        // Méthodes pour calculer la couleur en fonction de la température en kelvin
        static const Vector3 TemperatureColor(float t) ;
        static float GetTemperatureColor(const Vector3& col);

};

class planete : public Astre {
    public:
        Vector3 rotationSpeed;
        float AxialTilt; // Influence l'angle de rotation de la planète sur elle même
        float OrbitalPeriod;

        virtual std::unique_ptr<Object3D> clone() const override {
            return std::make_unique<planete>(*this);
        }

        planete(const Vector3& position, float mass, float radius, const Vector3& rotationSpeed, float AXT, const Vector3& color = Vector3::Zero, const Vector3& initialVelocity = Vector3::Zero) 
            : Astre(position, mass, radius, color, initialVelocity), rotationSpeed(rotationSpeed), AxialTilt(AXT) {}
};

class Satellite : public Astre {
    public:  
        virtual std::unique_ptr<Object3D> clone() const override {
            return std::make_unique<Satellite>(*this);
        }

        Satellite (const Vector3& position, float mass, float radius, const Vector3& color = Vector3::Zero, const Vector3& initialVelocity = Vector3::Zero) : Astre(position, mass, radius, color, initialVelocity) {}
};

