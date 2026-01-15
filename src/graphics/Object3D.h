#pragma once
#include <vector>
#include "core/math/Vector3.h"
#include "core/math/Matrice4x4.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector3 color;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

class Transform {
    public:
        Vector3 position;
        Vector3 rotation;   // angles d’Euler (rad)
        Vector3 scale;

        Transform();
        Matrice4 GetMatrix() const;
};

class Object3D {
    public:
        Mesh mesh;
        Transform transform;

        Object3D(const Mesh& mesh) : mesh(mesh) {}

        void Translate(const Vector3& t);
        void Rotate(const Vector3& r);
        void Scale(const Vector3& s);
};