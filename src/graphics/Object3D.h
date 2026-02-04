#pragma once
#include <vector>
#include <memory>
#include "../core/math/Vector2.h"
#include "../core/math/Vector3.h"
#include "../core/math/Matrice4x4.h"
#include "Material.h"

// Définition de la structure Vertex(point en 3d) 
struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector3 color;
};

// Définition de la structure VertexTransformed (point transformé pour l'écran)
struct VertexTransformed {
    point clipPosition;   // position homogène (AVANT division par W)
    Vector2 screenCoord;  // position écran (APRÈS division par W)
    Vector3 color;       // couleur du vertex
    float depth;          // z normalisé pour depth buffer
    float invW;          // 1/w pour interpolation perspective correcte
    Vector3 normal_view;        // normale en espace vue
    Vector3 normal_over_w;      // normale / w pour interpolation perspective correcte
    bool valid = true;  // indique si le vertex est valide (dans le frustum)
};

// Définition de la structure Mesh(ensemble de vertex formant une forme 3D)
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

// Constructeur de forme d'objet 3D
class MeshBuilder {
    public:
        // Formes basiques
        static Mesh CreateTriangle();
        static Mesh CreateQuad(float width, float height);
        static Mesh CreateCube(float size);
        
        // Sphère avec couleur personnalisable (coeu moteur)
        static Mesh CreateSphere(float radius, int sectorCount, int stackCount, const Vector3& color = Vector3(0.8f, 0.8f, 1.0f));
        
        // Sol
        static Mesh CreatePlane(float width, float height, int subdivisionsWidth, int subdivisionsHeight);
};

// Définition de la classe Transform (position, rotation, échelle)
class Transform {
    public:
        Vector3 position;
        Vector3 rotation;   // angles d'Euler (rad)
        Vector3 scale;

        Transform();
        Transform(const Vector3& pos) : position(pos), rotation(Vector3::Zero), scale(Vector3::One) {}
        Matrice4 GetMatrix() const;
};

// Définition de la classe Object3D
class Object3D {
    public:
        Mesh mesh;
        Transform transform;
        Material material;  // Matériau de l'objet
        int id;

        // Constructeur avec mesh uniquement (matériau par défaut)
        Object3D(const Vector3& position=Vector3::Zero) : transform(position), material(Material::White()) {}

        // Constructeur avec mesh uniquement (matériau par défaut)
        Object3D(const Mesh& mesh, const Vector3& position=Vector3::Zero) : mesh(mesh), transform(position), material(Material::White()) {}
        
        // Constructeur avec matériau
        Object3D(const Mesh& mesh, const Material& mat, const Vector3& position=Vector3::Zero) : mesh(mesh), transform(position), material(mat) {}

        virtual ~Object3D() = default;

        virtual std::unique_ptr<Object3D> clone() const {
            return std::make_unique<Object3D>(*this);
        }

        void Translate(const Vector3& t);
        void Rotate(const Vector3& r);
        void Scale(const Vector3& s);
        
        // Méthodes pour le matériau
        void SetMaterial(const Material& mat) { material = mat; }
        Material& GetMaterial() { return material; }
        const Material& GetMaterial() const { return material; }
        
        // Méthodes pratiques pour l'émission
        void SetEmissive(bool emissive, float intensity = 1.0f) {
            material.isEmissive = emissive;
            material.emissiveIntensity = intensity;
        }
        
        void SetColor(const Vector3& color) {
            material.albedoColor = color;
            material.emissiveColor = color;
        }

        bool operator==(const Object3D& other) const {
            return id == other.id;
        }
};