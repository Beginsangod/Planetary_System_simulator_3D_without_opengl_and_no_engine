#pragma once
#include "Object3D.h"
#include "Camera.h"
#include <string>

// Définition de la classe Scene
class Scene {
    private:
        std::vector<Object3D> objects;
        Camera camera;
        std::string sceneName;
        bool activ;

    public:
        Scene();
        ~Scene();

        void addObject(const Object3D& obj) { objects.push_back(obj); };
        void addCube(float size);
        void addQuad(float w, float h);
        void addTriangle(float size, const Vector3& position = Vector3::Zero);
        void deleteObject(const Object3D& obj);
        void clearObjects() { objects.clear(); };

        const std::vector<Object3D>& getObjects() const { return objects; }
        const Camera& _Camera() const { return camera; }
};