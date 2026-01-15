#pragma once
#include <vector>
#include "core/math/Matrice4x4.h"
#include "Object3D.h"

class Camera {
    private:
        float fov;
        float aspect;
        float near;
        float far;
        Transform transform;
        Matrice4 viewMatrix;
        Matrice4 projectionMatrix;

    public:
        Camera();
        ~Camera();

        Matrice4 LookAt();
};