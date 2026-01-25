#pragma once 
#include <cmath>

class Vector3 {
    public:
        float x;
        float y;
        float z;

        //Constructeur Vector
        Vector3();
        Vector3(float x, float y, float z): x(x), y(y), z(z) {};
        Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {};

        // Opérateurs classique 
        Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
        Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
        Vector3 operator*(const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
        Vector3 operator/(const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }

        // Opération scalaire
        Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

        // Opérateurs d'affectation
        Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        Vector3& operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
        Vector3& operator/=(const Vector3& other) { x /= other.x; y /= other.y; z /= other.z;  return *this; }

        // Opérateurs de comparaison
        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const Vector3& other) const { return x != other.x || y != other.y || z != other.z; }

        // Opérations vectorielles
        float NormeVector3() { return std::sqrt(x * x + y * y + z * z); };
        Vector3 Normalisation();
        Vector3 Prodv(const Vector3& other) const { return Vector3((y*other.z - z*other.y),(z*other.x - x*other.z),(x*other.y - y*other.x));};
        float Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; };

        // Vecteurs statiques communs ; constantes
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Left;
        static const Vector3 Right;
        static const Vector3 Front;
        static const Vector3 Back;
};
