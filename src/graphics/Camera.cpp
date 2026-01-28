#include "Camera.h"
#include "../core/Event/handleEvent.h"
#include <algorithm>

// fov en degrés, aspect ratio 16:9, near et far clipping planes
// IMPORTANT: yaw=0 signifie regarder vers -Z (dans l'écran)
Camera::Camera(): 
    fov(60), 
    aspect(1.77f), 
    near(0.1f), 
    far(100.0f), 
    moveSpeed(5.0f), 
    sprintMultiplier(2.0f),
    mouseSensibility(0.001f), 
    yaw(0.0f), 
    pitch(0.0f), 
    zoomSpeed(5.0f),
    mode(CameraMode::FREE_FLY),
    groundHeight(1.7f)  // Hauteur d'un humain (~1.7m)
{
    transform.position = Vector3(0, groundHeight, 5);
    transform.rotation = Vector3::Zero;
};

// Calcul de la matrice de vue
Matrice4 Camera::GetViewMatrix() {
    
    Matrice4 T = Matrice4::Translation(-transform.position.x, -transform.position.y, -transform.position.z);
    Matrice4 Rx = Matrice4::RotationX(-pitch);
    Matrice4 Ry = Matrice4::RotationY(-yaw);  
    
    return T * Ry * Rx;
}

//Oriente la caméra pour regarder un objet 3D spécifique
void Camera::LookAt(Object3D obj) {
// 1. Calcul de la direction caméra -> cible
    Vector3 direction = obj.transform.position - transform.position;
    direction = direction.Normalisation();

// 2. Calcul des angles d'Euler à partir du vecteur direction
    // Yaw : rotation autour de Y (gauche / droite)
    yaw = std::atan2(direction.x, -direction.z);
    // Pitch : rotation autour de X (haut / bas)
    pitch = std::asin(direction.y);
    
    // 3. Mise à jour de la rotation de la caméra
    transform.rotation.x = pitch;
    transform.rotation.y = yaw;
    transform.rotation.z = 0.0f;
}

// Calcul de la matrice de projection
Matrice4 Camera::GetProjectionMatrix() {
    Matrice4 projectionMatrix;

    // Calcul de la matrice de projection perspective
    float fovRad = fov * (3.14159265f / 180.0f); // Conversion en radians
    projectionMatrix = Matrice4::Perspective(fovRad, aspect, near, far);
    return projectionMatrix;
}

// Obtenir le vecteur forward
Vector3 Camera::GetForward() const {
    // En mode GROUND_WALK, on ignore le pitch pour le déplacement
    if (mode == CameraMode::GROUND_WALK) {
        Vector3 forward(-std::sin(yaw), 0.0f, -std::cos(yaw));
        return forward.Normalisation();
    } else {
        Vector3 forward(-std::sin(yaw), -std::sin(pitch), -std::cos(yaw) * std::cos(pitch));
        return forward.Normalisation();
    }
}

// Obtenir le vecteur right
Vector3 Camera::GetRight() const {
    Vector3 forward = GetForward();
    return forward.Prodv(Vector3::Up).Normalisation();
}

// Obtenir le vecteur up
Vector3 Camera::GetUp() const {
    Vector3 forward = GetForward();
    Vector3 right = GetRight();
    return right.Prodv(forward).Normalisation();
}

// Basculer entre les modes
void Camera::ToggleMode() {
    if (mode == CameraMode::FREE_FLY) {
        mode = CameraMode::GROUND_WALK;
        // Ajuster la hauteur au sol
        transform.position.y = groundHeight;
    } else {
        mode = CameraMode::FREE_FLY;
    }
}

// Traitement du clavier (ZQSD ou WASD) - AMÉLIORÉ
void Camera::ProcessKeyboard(float deltaTime) {
    // Déterminer la vitesse (avec sprint)
    float velocity = moveSpeed * deltaTime;
    if (Keyboard::IsKeyDown(KeyCode::LeftShift)) {
        velocity *= sprintMultiplier;
    }
    
    // Obtenir les vecteurs directionnels (dépend du mode)
    Vector3 forward = GetForward();
    Vector3 right = GetRight();
    Vector3 up = GetUp();
    
    // Déplacement avant/arrière (Z/S ou W/S)
    if (Keyboard::IsKeyDown(KeyCode::W) || Keyboard::IsKeyDown(KeyCode::Z)) {
        transform.position += forward * velocity;
    }
    if (Keyboard::IsKeyDown(KeyCode::S)) {
        transform.position -= forward * velocity;
    }
    
    // Déplacement gauche/droite (Q/D ou A/D)
    if (Keyboard::IsKeyDown(KeyCode::D)) {
        transform.position -= right * velocity;
    }
    if (Keyboard::IsKeyDown(KeyCode::A) || Keyboard::IsKeyDown(KeyCode::Q)) {
        transform.position += right * velocity;
    }
    
    // Déplacement vertical (Espace/Ctrl)
    // En mode GROUND_WALK, on ne peut pas monter/descendre
    if (mode == CameraMode::FREE_FLY) {
        if (Keyboard::IsKeyDown(KeyCode::Down)) {
            transform.position += Vector3::Up * velocity;
        }
        if (Keyboard::IsKeyDown(KeyCode::Up)) {
            transform.position -= Vector3::Up * velocity;
        }
    } else {
        // En mode GROUND_WALK, maintenir la hauteur constante
        transform.position.y = groundHeight;
    }

    // Mettre à jour la rotation du transform
    transform.rotation.x = pitch;
    transform.rotation.y = yaw;
    transform.rotation.z = 0.0f;
}

// Traitement de la souris pour regarder autour
void Camera::ProcessMouse(float deltaX, float deltaY) {
    yaw += deltaX * mouseSensibility;
    pitch -= deltaY * mouseSensibility;  // Inversé pour que haut = regarder haut

    // Limiter le pitch pour éviter le gimbal lock
    const float maxPitch = 89.0f * (3.14159265f / 180.0f);  // 89 degrés en radians
    pitch = std::clamp(pitch, -maxPitch, maxPitch);
    
    // Mettre à jour la rotation du transform
    transform.rotation.x = pitch;
    transform.rotation.y = yaw;
    transform.rotation.z = 0.0f;  // Pas de roulis
}

// Setter pour le FOV avec validation
void Camera::SetFOV(float newFov) {
    fov = std::clamp(newFov, 10.0f, 120.0f);
}