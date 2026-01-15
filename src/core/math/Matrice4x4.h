#pragma once
#include "Vector3.h"

class Matrice4 {
    public:
        float mat[4][4];

        Matrice4();
        Matrice4(const Matrice4& other);

        // Matrice identité
        static Matrice4 Identity();
        
        // Matrices de transformation
        static Matrice4 Translation(float x, float y, float z);
        static Matrice4 Scaling(float sx, float sy, float sz);
        static Matrice4 RotationX(float angle);
        static Matrice4 RotationY(float angle);
        static Matrice4 RotationZ(float angle);

        //Projection ==/== Caméra
        static Matrice4 lookAt( const Vector3& eye, const Vector3& target, const Vector3& up);
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

// classe point homogène
class point: public Vector3{
    public:
        float ph[4];

        point(){};
        point(Vector3 v, int n){ ph[0] = x; ph[1] = y; ph[2] = z; ph[3] = n; }
        point(const point& p) {}

        //Appliqué les changements
        point update(){ x = ph[0]; y = ph[1]; z = ph[2]; }
        
        point operator*(const Matrice4& p) const;
};