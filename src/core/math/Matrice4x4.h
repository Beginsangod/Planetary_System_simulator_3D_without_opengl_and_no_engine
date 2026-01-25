#pragma once
#include "Vector3.h"

class Matrice4 {
    public:
        float mat[4][4];

        Matrice4();
        // Matrice4(const Matrice4& other);

        // Matrice identité
        static Matrice4 Identity();
        
        // Matrices de transformation
        static Matrice4 Translation(float x, float y, float z);
        static Matrice4 Scaling(float sx, float sy, float sz);
        static Matrice4 RotationX(float angle);
        static Matrice4 RotationY(float angle);
        static Matrice4 RotationZ(float angle);

        //Projection ==/== Caméra
        static Matrice4 Perspective( float fov, float aspect, float near, float far);

        //Application
        Vector3 TransformPoint(const Vector3& v);
        Vector3 TransformDirection(const Vector3& v);

        // Opération matricielle
        Matrice4 Transpose() const;
        Matrice4 Inverse() const;

        //Multiplication
        Matrice4 operator*(const Matrice4& other) const;
};

// Définition de la classe point homogène
class point {
public:
    float x, y, z, w;

    // Constructeur par défaut
    point() : x(0), y(0), z(0), w(1) {}

    // Point (w = 1)
    point(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}

    // Direction (w = 0)
    static point Direction(const Vector3& v) {
        point p;
        p.x = v.x;
        p.y = v.y;
        p.z = v.z;
        p.w = 0.0f;
        return p;
    }

    // Multiplication d'un point homogène par une matrice 4x4
    point operator*(const Matrice4& m) const;
};
    