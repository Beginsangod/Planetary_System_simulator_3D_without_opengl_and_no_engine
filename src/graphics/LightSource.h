#pragma once
#include "../core/math/Vector3.h"

// STRUCTURE DE SOURCE LUMINEUSE

struct LightSource {
    Vector3 position;      // Position de la lumière dans le monde
    Vector3 color;         // Couleur de la lumière (RGB)
    float intensity;       // Intensité de la lumière
    float radius;          // Rayon d'influence (optionnel)
    
    // Constructeur par défaut
    LightSource() 
        : position(Vector3::Zero)
        , color(Vector3(1.0f, 1.0f, 1.0f))
        , intensity(1.0f)
        , radius(50.0f)
    {}
    
    // Constructeur avec paramètres
    LightSource(const Vector3& pos, const Vector3& col, float inten, float rad = 50.0f)
        : position(pos)
        , color(col)
        , intensity(inten)
        , radius(rad)
    {}
    
    // Calculer l'atténuation à une distance donnée
    float CalculateAttenuation(float distance) const {
        // Formule physique : 1 / (constant + linear * d + quadratic * d²)
        // Constantes ajustées pour un bon rendu visuel
        const float constant = 1.0f;
        const float linear = 0.09f;
        const float quadratic = 0.032f;
        
        return 1.0f / (constant + linear * distance + quadratic * distance * distance);
    }
    
    // Vérifier si un point est dans la zone d'influence
    bool IsInRange(const Vector3& point) const {
        float distance = (position - point).Magnitude();
        return distance <= radius;
    }
};