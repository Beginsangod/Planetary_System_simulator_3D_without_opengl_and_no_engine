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
        void addCube(float size, const Vector3& position);
        void addQuad(float w, float h, const Vector3& position);
        void addTriangle(float size, const Vector3& position = Vector3::Zero);
        void addPlane(float width, float height, int subW, int subH, const Vector3& position);
        void addSphere(float radius, int sectorCount, int stackCount, const Vector3& position = Vector3::Zero, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));
        void deleteObject(const Object3D& obj);
        void clearObjects() { objects.clear(); };
        void SetCameraAspectRatio(float aspect) { camera.SetAspectRatio(aspect); };

        const std::vector<Object3D>& getObjects() const { return objects; };
        Camera& GetCamera() { return camera; }
        const Camera& _Camera() const { return camera; }
};