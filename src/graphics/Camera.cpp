#include "Camera.h"

// fov en degrés, aspect ratio 16:9, near et far clipping planes
Camera::Camera(): fov(60), aspect(1.77f), near(0.1f), far(100.0f) {
    transform.position = Vector3(0, 0, 10);
    transform.rotation = Vector3::Zero;
};

// Calcul de la matrice de vue
Matrice4 Camera::GetViewMatrix() {
    Matrice4 viewMatrix;

    // Calcul de la matrice de vue en fonction de la position et de la rotation de la caméra
    Matrice4 T = Matrice4::Translation(-transform.position.x, -transform.position.y, -transform.position.z);
    Matrice4 Rx = Matrice4::RotationX(-transform.rotation.x);
    Matrice4 Ry = Matrice4::RotationY(-transform.rotation.y);
    Matrice4 Rz = Matrice4::RotationZ(-transform.rotation.z);

    viewMatrix = T * Rx * Ry * Rz;  // au lieu de Rz * Ry * Rx * T
    return viewMatrix;
}

//Oriente la caméra pour regarder un objet 3D spécifique
void Camera::LookAt(Object3D obj) {
// 1. Calcul de la direction caméra -> cible
    Vector3 direction = obj.transform.position - transform.position;
    direction = direction.Normalisation();

// 2. Calcul des angles d’Euler à partir du vecteur direction
    // Yaw : rotation autour de Y (gauche / droite)
    float yaw = std::atan2(direction.x, -direction.z);
    // Pitch : rotation autour de X (haut / bas)
    float pitch = std::asin(direction.y);
    // Roll : toujours 0 pour une caméra classique
    float roll = 0.0f;
    // 3. Mise à jour de la rotation de la caméra
    transform.rotation = Vector3(pitch, yaw, roll);
}

// Calcul de la matrice de projection
Matrice4 Camera::GetProjectionMatrix() {
    Matrice4 projectionMatrix;

    // Calcul de la matrice de projection perspective
    float fovRad = fov * (3.14159265f / 180.0f); // Conversion en radians
    projectionMatrix = Matrice4::Perspective(fovRad, aspect, near, far);
    return projectionMatrix;
}