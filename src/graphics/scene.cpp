#include "scene.h"
#include <algorithm>

//contructeur scène
Scene::Scene() : sceneName("DefaultScene"), activ(true) {}

// destructeur scène
Scene::~Scene() {
    clearObjects();
}

// ajout d'un cube à la scène pour test
void Scene::addCube(float size, const Vector3& position) {
    Mesh cubeMesh = MeshBuilder::CreateCube(size);
    Object3D cubeObject(cubeMesh);
    cubeObject.transform.position = position;
    addObject(cubeObject);
}

// ajout d'un quad à la scène pour test
void Scene::addQuad(float w, float h, const Vector3& position) {
    Mesh quadMesh = MeshBuilder::CreateQuad(w, h);
    Object3D quadObject(quadMesh);
    quadObject.transform.position = position;
    addObject(quadObject);
}

// ajout d'un triangle à la scène pour test
void Scene::addTriangle(float size, const Vector3& position) {
    Mesh triangleMesh = MeshBuilder::CreateTriangle();
    Object3D triangleObject(triangleMesh);
    addObject(triangleObject);
}

// ajout d'un plan à la scène
void Scene::addPlane(float width, float height, int subW, int subH, const Vector3& position) {
    Mesh planeMesh = MeshBuilder::CreatePlane(width, height, subW, subH);
    Object3D planeObject(planeMesh);
    planeObject.transform.position = position;
    addObject(planeObject);
}

//ajout d'une sphère à la scène
void Scene::addSphere(float radius, int sectorCount, int stackCount, const Vector3& position) {
    Mesh sphereMesh = MeshBuilder::CreateSphere(radius, sectorCount, stackCount);
    Object3D sphereObject(sphereMesh);
    sphereObject.transform.position = position;
    addObject(sphereObject);
}

// suppression d'un objet de la scène
void Scene::deleteObject(const Object3D& obj) {
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

