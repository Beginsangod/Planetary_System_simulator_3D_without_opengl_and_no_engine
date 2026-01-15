#include "Object3D.h"

Transform::Transform()
    : position(Vector3::Zero), rotation(Vector3::Zero), scale(Vector3::One) {}

Matrice4 Transform::GetMatrix() const
{
    Matrice4 T = Matrice4::Translation(position.x, position.y, position.z);
    Matrice4 Rx = Matrice4::RotationX(rotation.x);
    Matrice4 Ry = Matrice4::RotationY(rotation.y);
    Matrice4 Rz = Matrice4::RotationZ(rotation.z);
    Matrice4 S = Matrice4::Scaling(scale.x, scale.y, scale.z);

    return T * Rz * Ry * Rx * S;
}

void Object3D::Translate(const Vector3& t) {
    transform.position += t;
}

void Object3D::Rotate(const Vector3& r) {
    transform.rotation += r;
}

void Object3D::Scale(const Vector3& s) {
    transform.scale *= s;
}