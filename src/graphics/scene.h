#pragma once
#include "Object3D.h"
#include "Camera.h"
#include <string>
#include <memory>

// Définition de la classe Scene 
class Scene {
    protected:
        std::vector<std::unique_ptr<Object3D>> objects;
        Camera camera;
        bool activ;
        int n = 0;

    public:
        std::string sceneName;

        Scene();
        ~Scene();

        // Implémentation la copie profonde
        Scene(const Scene& other);
        Scene& operator=(const Scene& other);
        
        // Méthodes test affichage et ajout
        void addCube(float size, const Vector3& position);
        void addQuad(float w, float h, const Vector3& position);
        void addTriangle(float size, const Vector3& position = Vector3::Zero);
        void addPlane(float width, float height, int subW, int subH, const Vector3& position);
        void addSphereSolid(float radius, int sectorCount, int stackCount, const Vector3& position = Vector3::Zero, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));
        void addSphereLight(float radius, int sectorCount, int stackCount, const Vector3& position = Vector3::Zero, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f), float intensity = 5.0f);
        
        // Gestion Objet scène
        void addObject(std::unique_ptr<Object3D> obj) { obj->id = n; n++; objects.push_back(std::move(obj));};
        void deleteObject(std::unique_ptr<Object3D> obj);
        void clearObjects() { objects.clear(); n = 0; };
        void SetCameraAspectRatio(float aspect) { camera.SetAspectRatio(aspect); };

        const std::vector<std::unique_ptr<Object3D>>& getObjects() const { return objects; };
        Camera& GetCamera() { return camera; }
        const Camera& _Camera() const { return camera; }
};