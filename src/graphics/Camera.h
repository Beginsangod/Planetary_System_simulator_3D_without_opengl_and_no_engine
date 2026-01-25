#pragma once
#include "Object3D.h"

class Camera {
    private:
        float fov;
        float aspect; 
        float near;
        float far;
        Transform transform;

    public:
        Camera();

        Matrice4 GetViewMatrix();
        Matrice4 GetProjectionMatrix();
        void LookAt(Object3D obj);
};