#include "Vector3.h"

Vector3::Vector3():x(0), y(0), z(0) {};

// Définition des constantes statiques
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Up(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Front(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Back(0.0f, 0.0f, 1.0f);

Vector3 Vector3::Normalisation(){
    float norme = this->NormeVector3();
    return norme > 0 ? *this / norme : Zero;
}
