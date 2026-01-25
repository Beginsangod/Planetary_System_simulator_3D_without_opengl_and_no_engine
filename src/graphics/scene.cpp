#include "scene.h"
#include <algorithm>

//contructeur scène
Scene::Scene() : sceneName("DefaultScene"), activ(true) {}

// destructeur scène
Scene::~Scene() {
    clearObjects();
}

// ajout d'un cube à la scène pour test
void Scene::addCube(float size) {
    Mesh cubeMesh = MeshBuilder::CreateCube(size);
    Object3D cubeObject(cubeMesh);
    addObject(cubeObject);
}

// ajout d'un quad à la scène pour test
void Scene::addQuad(float w, float h) {
    Mesh quadMesh = MeshBuilder::CreateQuad(w, h);
    Object3D quadObject(quadMesh);
    addObject(quadObject);
}

// ajout d'un triangle à la scène pour test
void Scene::addTriangle(float size, const Vector3& position) {
    Mesh triangleMesh = MeshBuilder::CreateTriangle();
    Object3D triangleObject(triangleMesh);
    addObject(triangleObject);
}

// suppression d'un objet de la scène
void Scene::deleteObject(const Object3D& obj) {
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

