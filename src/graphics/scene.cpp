#include "scene.h"
#include <algorithm>

//contructeur scène
Scene::Scene() : sceneName("DefaultScene"), activ(true) {}

// destructeur scène
Scene::~Scene() {
    clearObjects();
}

Scene::Scene(const Scene& other) : activ(other.activ), n(other.n), sceneName(other.sceneName), camera(other.camera) {
    // Copie profonde des objets
    for (const auto& obj : other.objects) {
        objects.push_back(obj->clone());  // Utilise la méthode virtuelle clone()
    }
}

Scene& Scene::operator=(const Scene& other) {
    if (this != &other) {
        activ = other.activ;
        n = other.n;
        sceneName = other.sceneName;
        camera = other.camera; 
        objects.clear();
        for (const auto& obj : other.objects) {
            objects.push_back(obj->clone());  // Utilise la méthode virtuelle clone()
        }
    }
    return *this;
}

// ajout d'un cube à la scène pour test
void Scene::addCube(float size, const Vector3& position) {
    Mesh cubeMesh = MeshBuilder::CreateCube(size);
    auto cubeObject = std::make_unique<Object3D>(cubeMesh);
    cubeObject->transform.position = position;
    addObject(std::move(cubeObject));
}

// ajout d'un quad à la scène pour test
void Scene::addQuad(float w, float h, const Vector3& position) {
    Mesh quadMesh = MeshBuilder::CreateQuad(w, h);
    auto quadObject = std::make_unique<Object3D>(quadMesh);
    quadObject->transform.position = position;
    addObject(std::move(quadObject));
}

// ajout d'un triangle à la scène pour test
void Scene::addTriangle(float size, const Vector3& position) {
    Mesh triangleMesh = MeshBuilder::CreateTriangle();
    auto triangleObject = std::make_unique<Object3D>(triangleMesh);
    addObject(std::move(triangleObject));
}

// ajout d'un plan à la scène
void Scene::addPlane(float width, float height, int subW, int subH, const Vector3& position) {
    Mesh planeMesh = MeshBuilder::CreatePlane(width, height, subW, subH);
    auto planeObject = std::make_unique<Object3D>(planeMesh);
    planeObject->transform.position = position;
    addObject(std::move(planeObject));
}

//ajout d'une sphère à la scène
void Scene::addSphereSolid(float radius, int sectorCount, int stackCount, const Vector3& position, const Vector3& color) {
    Mesh sphereMesh = MeshBuilder::CreateSphere(radius, sectorCount, stackCount, color);
    auto sphereObject = std::make_unique<Object3D>(sphereMesh);
    sphereObject->material = Material(color);
    sphereObject->transform.position = position;
    addObject(std::move(sphereObject));
}

// ajout d'une sphère lumineuse émissive à la scène
void Scene::addSphereLight(float radius, int sectorCount, int stackCount, const Vector3& position, const Vector3& color, float intensity) {
    Mesh sphereMesh = MeshBuilder::CreateSphere(radius, sectorCount, stackCount, color);
    auto sphereObject = std::make_unique<Object3D>(sphereMesh);
    sphereObject->transform.position = position;
    // Définir le matériau émissif
    Material emissiveMaterial = Material::EmissiveWhite(intensity); // Intensité d'émission personnalisée
    emissiveMaterial.emissiveColor = color; // Couleur personnalisée
    sphereObject->SetMaterial(emissiveMaterial);
    addObject(std::move(sphereObject));
}

// suppression d'un objet de la scène
void Scene::deleteObject(std::unique_ptr<Object3D> obj) {
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
    n--;
}

