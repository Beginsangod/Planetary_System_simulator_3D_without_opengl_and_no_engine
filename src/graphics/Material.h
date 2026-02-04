#pragma once
#include "../core/math/Vector3.h"

// SYSTÈME DE MATÉRIAUX

struct Material {
    Vector3 albedoColor;      // Couleur de base
    Vector3 emissiveColor;    // Couleur émissive (si émetteur de lumière)
    float emissiveIntensity;  // Intensité de l'émission (0 = pas d'émission)
    float roughness;          // Rugosité (0 = lisse, 1 = rugueux)
    float metallic;           // Métallique (0 = diélectrique, 1 = métal)
    bool isEmissive;          // Est-ce que l'objet émet de la lumière ?
    
    // Constructeur par défaut : matériau blanc standard
    Material() 
        : albedoColor(0.8f, 0.8f, 0.8f)
        , emissiveColor(1.0f, 1.0f, 1.0f)
        , emissiveIntensity(0.0f)
        , roughness(0.5f)
        , metallic(0.0f)
        , isEmissive(false)
    {}
    
    // Constructeur avec couleur
    Material(const Vector3& color) 
        : albedoColor(color)
        , emissiveColor(color)
        , emissiveIntensity(0.0f)
        , roughness(0.5f)
        , metallic(0.0f)
        , isEmissive(false)
    {}
    
    // Constructeur complet
    Material(const Vector3& color, bool emissive, float intensity = 1.0f)
        : albedoColor(color)
        , emissiveColor(color)
        , emissiveIntensity(emissive ? intensity : 0.0f)
        , roughness(0.5f)
        , metallic(0.0f)
        , isEmissive(emissive)
    {}
    
    // ========== MATÉRIAUX PRÉDÉFINIS ==========
    
    // Matériaux standards
    static Material White() {
        return Material(Vector3(1.0f, 1.0f, 1.0f));
    }
    
    static Material Red() {
        return Material(Vector3(1.0f, 0.0f, 0.0f));
    }
    
    static Material Green() {
        return Material(Vector3(0.0f, 1.0f, 0.0f));
    }
    
    static Material Blue() {
        return Material(Vector3(0.0f, 0.0f, 1.0f));
    }
    
    static Material Yellow() {
        return Material(Vector3(1.0f, 1.0f, 0.0f));
    }
    
    static Material Cyan() {
        return Material(Vector3(0.0f, 1.0f, 1.0f));
    }
    
    static Material Magenta() {
        return Material(Vector3(1.0f, 0.0f, 1.0f));
    }
    
    // Matériaux émissifs (lumières)
    static Material EmissiveWhite(float intensity = 1.0f) {
        return Material(Vector3(1.0f, 1.0f, 1.0f), true, intensity);
    }
    
    static Material EmissiveRed(float intensity = 1.0f) {
        return Material(Vector3(1.0f, 0.0f, 0.0f), true, intensity);
    }
    
    static Material EmissiveGreen(float intensity = 1.0f) {
        return Material(Vector3(0.0f, 1.0f, 0.0f), true, intensity);
    }
    
    static Material EmissiveBlue(float intensity = 1.0f) {
        return Material(Vector3(0.0f, 0.0f, 1.0f), true, intensity);
    }
    
    static Material EmissiveYellow(float intensity = 1.0f) {
        return Material(Vector3(1.0f, 1.0f, 0.0f), true, intensity);
    }
    
    static Material EmissiveOrange(float intensity = 1.0f) {
        return Material(Vector3(1.0f, 0.5f, 0.0f), true, intensity);
    }
    
    // Obtenir la couleur finale (avec émission)
    Vector3 GetFinalColor() const {
        if (isEmissive && emissiveIntensity > 0.0f) {
            return emissiveColor * emissiveIntensity;
        }
        return albedoColor;
    }
};
