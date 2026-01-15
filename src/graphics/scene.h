#pragma once
#include <vector>
#include "Object3D.h"
#include <string>

class Scene {
    private:
        std::vector<Object3D> objects;
        std::string sceneName;
        bool activ;

    public:
        Scene();
        ~Scene();

        void addObject(const Object3D& obj) { objects.push_back(obj); };
        void deleteObject(const Object3D& obj);
        void clearObjects() { objects.clear(); };

        const std::vector<Object3D>& getObjects() const { return objects; }
};