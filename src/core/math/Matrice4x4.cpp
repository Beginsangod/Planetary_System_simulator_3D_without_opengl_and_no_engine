#include "Matrice4x4.h"

Matrice4::Matrice4() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] = (i == j) ? 1.0f : 0.0f;
};

Matrice4 Matrice4::Identity(){
    return Matrice4();
};

Matrice4 Matrice4::Translation(float x, float y,float z) {
    Matrice4 mati = Identity();
    mati.mat[0][3] = x;
    mati.mat[1][3] = y;
    mati.mat[2][3] = z;
    return mati;
}

Matrice4 Matrice4::RotationX(float angle) {
    Matrice4 mati = Identity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    
    mati.mat[1][1] = cosA;
    mati.mat[1][2] = -sinA;
    mati.mat[2][1] = sinA;
    mati.mat[2][2] = cosA;
    
    return mati;
}

Matrice4 Matrice4::RotationY(float angle) {
    Matrice4 mati = Identity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    
    mati.mat[0][0] = cosA;
    mati.mat[0][2] = sinA;
    mati.mat[2][0] = -sinA;
    mati.mat[2][2] = cosA;
    
    return mati;
}

Matrice4 Matrice4::RotationZ(float angle) {
    Matrice4 mati = Identity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    
    mati.mat[0][0] = cosA;
    mati.mat[0][1] = -sinA;
    mati.mat[1][0] = sinA;
    mati.mat[1][1] = cosA;
    
    return mati;
}

Matrice4 Matrice4::Scaling(float sx, float sy, float sz) {
    Matrice4 mati = Identity(); 
    mati.mat[0][0] = sx;
    mati.mat[1][1] = sy;
    mati.mat[2][2] = sz;
    return mati;
}

Vector3 Matrice4::TransformPoint(const Vector3& v){
    point p(v, 1);
    p = p * *this;
    return p;
}

Vector3 Matrice4::TransformDirection(const Vector3& v){
    point p(v, 0);
    p = p * *this;
    return p;
}

Matrice4 Matrice4::Transpose() const{
    Matrice4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.mat[j][i] = mat[i][j];
        }
    }
    return res;
}

Matrice4 Matrice4::Inverse() const
{
    Matrice4 inv;
    float det;
    float invOut[16];

    const float* m = &mat[0][0];

    invOut[0] = m[5]  * m[10] * m[15] -
                m[5]  * m[11] * m[14] -
                m[9]  * m[6]  * m[15] +
                m[9]  * m[7]  * m[14] +
                m[13] * m[6]  * m[11] -
                m[13] * m[7]  * m[10];

    invOut[4] = -m[4]  * m[10] * m[15] +
                 m[4]  * m[11] * m[14] +
                 m[8]  * m[6]  * m[15] -
                 m[8]  * m[7]  * m[14] -
                 m[12] * m[6]  * m[11] +
                 m[12] * m[7]  * m[10];

    invOut[8] = m[4]  * m[9] * m[15] -
                m[4]  * m[11] * m[13] -
                m[8]  * m[5] * m[15] +
                m[8]  * m[7] * m[13] +
                m[12] * m[5] * m[11] -
                m[12] * m[7] * m[9];

    invOut[12] = -m[4]  * m[9] * m[14] +
                  m[4]  * m[10] * m[13] +
                  m[8]  * m[5] * m[14] -
                  m[8]  * m[6] * m[13] -
                  m[12] * m[5] * m[10] +
                  m[12] * m[6] * m[9];

    invOut[1] = -m[1]  * m[10] * m[15] +
                 m[1]  * m[11] * m[14] +
                 m[9]  * m[2] * m[15] -
                 m[9]  * m[3] * m[14] -
                 m[13] * m[2] * m[11] +
                 m[13] * m[3] * m[10];

    invOut[5] = m[0]  * m[10] * m[15] -
                m[0]  * m[11] * m[14] -
                m[8]  * m[2] * m[15] +
                m[8]  * m[3] * m[14] +
                m[12] * m[2] * m[11] -
                m[12] * m[3] * m[10];

    invOut[9] = -m[0]  * m[9] * m[15] +
                 m[0]  * m[11] * m[13] +
                 m[8]  * m[1] * m[15] -
                 m[8]  * m[3] * m[13] -
                 m[12] * m[1] * m[11] +
                 m[12] * m[3] * m[9];

    invOut[13] = m[0]  * m[9] * m[14] -
                 m[0]  * m[10] * m[13] -
                 m[8]  * m[1] * m[14] +
                 m[8]  * m[2] * m[13] +
                 m[12] * m[1] * m[10] -
                 m[12] * m[2] * m[9];

    invOut[2] = m[1]  * m[6] * m[15] -
                m[1]  * m[7] * m[14] -
                m[5]  * m[2] * m[15] +
                m[5]  * m[3] * m[14] +
                m[13] * m[2] * m[7] -
                m[13] * m[3] * m[6];

    invOut[6] = -m[0]  * m[6] * m[15] +
                 m[0]  * m[7] * m[14] +
                 m[4]  * m[2] * m[15] -
                 m[4]  * m[3] * m[14] -
                 m[12] * m[2] * m[7] +
                 m[12] * m[3] * m[6];

    invOut[10] = m[0]  * m[5] * m[15] -
                 m[0]  * m[7] * m[13] -
                 m[4]  * m[1] * m[15] +
                 m[4]  * m[3] * m[13] +
                 m[12] * m[1] * m[7] -
                 m[12] * m[3] * m[5];

    invOut[14] = -m[0]  * m[5] * m[14] +
                  m[0]  * m[6] * m[13] +
                  m[4]  * m[1] * m[14] -
                  m[4]  * m[2] * m[13] -
                  m[12] * m[1] * m[6] +
                  m[12] * m[2] * m[5];

    invOut[3] = -m[1] * m[6] * m[11] +
                 m[1] * m[7] * m[10] +
                 m[5] * m[2] * m[11] -
                 m[5] * m[3] * m[10] -
                 m[9] * m[2] * m[7] +
                 m[9] * m[3] * m[6];

    invOut[7] = m[0] * m[6] * m[11] -
                m[0] * m[7] * m[10] -
                m[4] * m[2] * m[11] +
                m[4] * m[3] * m[10] +
                m[8] * m[2] * m[7] -
                m[8] * m[3] * m[6];

    invOut[11] = -m[0] * m[5] * m[11] +
                  m[0] * m[7] * m[9] +
                  m[4] * m[1] * m[11] -
                  m[4] * m[3] * m[9] -
                  m[8] * m[1] * m[7] +
                  m[8] * m[3] * m[5];

    invOut[15] = m[0] * m[5] * m[10] -
                 m[0] * m[6] * m[9] -
                 m[4] * m[1] * m[10] +
                 m[4] * m[2] * m[9] +
                 m[8] * m[1] * m[6] -
                 m[8] * m[2] * m[5];

    det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];

    if (det == 0)
        return Identity();

    det = 1.0f / det;

    for (int i = 0; i < 16; i++)
        inv.mat[i / 4][i % 4] = invOut[i] * det;

    return inv;
}

//fov: angle caméra; aspect: ration widthscreen/heightscreen; near: distance de visibilité min; far: Distance visibilté max 
Matrice4 Matrice4::Perspective(float fov, float aspect, float near, float far)
{
    Matrice4 p;
    float tanHalfFov = std::tan(fov * 0.5f);

    // remise à zéro
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            p.mat[i][j] = 0.0f;

    p.mat[0][0] = 1.0f / (aspect * tanHalfFov);
    p.mat[1][1] = 1.0f / tanHalfFov;

    p.mat[2][2] = -(far + near) / (far - near);
    p.mat[2][3] = -(2.0f * far * near) / (far - near);

    p.mat[3][2] = -1.0f;

    return p;
}

Matrice4 Matrice4::lookAt(const Vector3& eye,const Vector3& target,const Vector3& up)
{
    Vector3 z = (eye - target).Normalisation();   // axe arrière
    Vector3 x = up.Prodv(z).Normalisation();       // axe droite
    Vector3 y = z.Prodv(x);                     // axe haut

    Matrice4 view = Identity();

    view.mat[0][0] = x.x;
    view.mat[0][1] = x.y;
    view.mat[0][2] = x.z;
    view.mat[0][3] = -x.Dot(eye);

    view.mat[1][0] = y.x;
    view.mat[1][1] = y.y;
    view.mat[1][2] = y.z;
    view.mat[1][3] = -y.Dot(eye);

    view.mat[2][0] = z.x;
    view.mat[2][1] = z.y;
    view.mat[2][2] = z.z;
    view.mat[2][3] = -z.Dot(eye);

    return view;
}


Matrice4 Matrice4::operator*(const Matrice4& other) const {
    Matrice4 result;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.mat[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.mat[i][j] += mat[i][k] * other.mat[k][j];
            }
        }
    }
    
    return result;
}

point point::operator*(const Matrice4& m) const{ 
    point p1;   

    for (int i=0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p1.ph[i] += m.mat[i][j] * ph[j];
        }
    }

    p1.update();
    return p1;
}