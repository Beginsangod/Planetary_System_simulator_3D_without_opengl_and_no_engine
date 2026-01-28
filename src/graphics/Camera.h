#pragma once
#include "Object3D.h"

// Modes de déplacement de la caméra
enum class CameraMode {
    FREE_FLY,      // Vol libre (6 degrés de liberté)
    GROUND_WALK    // Marche au sol (déplacement sur le plan XZ uniquement)
};

class Camera {
    private:
        float fov;
        float aspect; 
        float near;
        float far;
        Transform transform;
        float moveSpeed;
        float sprintMultiplier;  // NOUVEAU : multiplicateur de vitesse en sprint
        float mouseSensibility;
        float yaw; // Rotation horizontale (en radians)
        float pitch; // Rotation verticale (en radians)
        float zoomSpeed; // Vitesse de zoom
        
        // NOUVEAU : Mode caméra et hauteur au sol
        CameraMode mode;
        float groundHeight;  // Hauteur par défaut au-dessus du sol en mode GROUND_WALK

    public:
        Camera();

        Matrice4 GetViewMatrix();
        Matrice4 GetProjectionMatrix();
        void LookAt(Object3D obj);

        // Méthodes pour déplacer la caméra
        void ProcessKeyboard(float deltaTime);
        void ProcessMouse(float deltaX, float deltaY);
        void SetMoveSpeed(float speed) { moveSpeed = speed; }
        void SetMouseSensitivity(float sensitivity) { mouseSensibility = sensitivity; }
        void SetZoomSpeed(float speed) { zoomSpeed = speed; }
        void SetSprintMultiplier(float multiplier) { sprintMultiplier = multiplier; }

        // NOUVEAU : Gestion du mode caméra
        void SetMode(CameraMode newMode) { mode = newMode; }
        CameraMode GetMode() const { return mode; }
        void ToggleMode();
        void SetGroundHeight(float height) { groundHeight = height; }
        float GetGroundHeight() const { return groundHeight; }

        // Getters/Setters pour le FOV (pour le zoom)
        float GetFOV() const { return fov; }
        void SetFOV(float newFov);

        // Getters/Setters pour accéder au transform
        Transform& GetTransform() { return transform; }
        const Transform& GetTransform() const { return transform; }

        // méthodes ratio aspect setters
        void SetAspectRatio(float newAspect) { aspect = newAspect; }
        
        // NOUVEAU : Obtenir les vecteurs directionnels
        Vector3 GetForward() const;
        Vector3 GetRight() const;
        Vector3 GetUp() const;
};