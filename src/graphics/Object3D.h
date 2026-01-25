#pragma once
#include <vector>
#include "../core/math/Vector2.h"
#include "../core/math/Vector3.h"
#include "../core/math/Matrice4x4.h"

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
        static Mesh CreateTriangle();
        static Mesh CreateQuad(float width, float height);
        static Mesh CreateCube(float size);
        static Mesh CreatePlane(float width, float height, int subdivisionsWidth, int subdivisionsHeight);
        static Mesh CreateSphere(float radius, int sectorCount, int stackCount);
};

// Définition de la classe Transform (position, rotation, échelle)
class Transform {
    public:
        Vector3 position;
        Vector3 rotation;   // angles d’Euler (rad)
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
        int id;

        Object3D(const Mesh& mesh, const Vector3& position=Vector3::Zero) : mesh(mesh), transform(position) {}

        void Translate(const Vector3& t);
        void Rotate(const Vector3& r);
        void Scale(const Vector3& s);

        bool operator==(const Object3D& other) const {
            return id == other.id;
        }
};