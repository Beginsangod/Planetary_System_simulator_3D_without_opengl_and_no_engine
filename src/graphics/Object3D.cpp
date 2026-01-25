#include "Object3D.h"

// Constructeur par défaut du Transform
Transform::Transform()
    : position(Vector3::Zero), rotation(Vector3::Zero), scale(Vector3::One) {}

// Obtention de la matrice de transformation complète(model matrix)
Matrice4 Transform::GetMatrix() const
{
    Matrice4 T = Matrice4::Translation(position.x, position.y, position.z);
    Matrice4 Rx = Matrice4::RotationX(rotation.x);
    Matrice4 Ry = Matrice4::RotationY(rotation.y);
    Matrice4 Rz = Matrice4::RotationZ(rotation.z);
    Matrice4 S = Matrice4::Scaling(scale.x, scale.y, scale.z);

    // CORRECTION : ordre inversé pour correspondre à ton opérateur *
    return S * Rx * Ry * Rz * T;  // au lieu de T * Rz * Ry * Rx * S
}

//Méthode de création de Triangle
Mesh MeshBuilder::CreateTriangle() {
    Mesh mesh;
    mesh.vertices = {
        Vertex{Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)},
        Vertex{Vector3( 0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)},
        Vertex{Vector3( 0.5f,  0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)}
    };
    mesh.indices = {0, 1, 2};
    return mesh;
}

// Méthode de création de Quad
Mesh MeshBuilder::CreateQuad(float width, float height) {
    Mesh mesh;

    float hw = width * 0.5f;
    float hh = height * 0.5f;

    mesh.vertices = {
        // bas gauche
        Vertex{ Vector3(-hw, -hh, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)},
        // bas droite
        Vertex{ Vector3(hw, -hh, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)},
        // haut droite
        Vertex{Vector3(hw, hh, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)},
        // haut gauche
        Vertex{ Vector3(-hw, hh, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)}
    };
    // Deux triangles (sens anti-horaire → face avant)
    mesh.indices = {
        0, 1, 2,
        0, 2, 3
    };
    return mesh;
}

void AddQuad( Mesh& mesh, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& normal){
    unsigned int startIndex = static_cast<unsigned int>(mesh.vertices.size());
    mesh.vertices.push_back( Vertex{ v0, normal, Vector3(1.0f, 1.0f, 1.0f) } );
    mesh.vertices.push_back( Vertex{ v1, normal, Vector3(1.0f, 1.0f, 1.0f) } );
    mesh.vertices.push_back( Vertex{ v2, normal, Vector3(1.0f, 1.0f, 1.0f) } );
    mesh.vertices.push_back( Vertex{ v3, normal, Vector3(1.0f, 1.0f, 1.0f) } );

    mesh.indices.push_back( startIndex + 0 );
    mesh.indices.push_back( startIndex + 1 );
    mesh.indices.push_back( startIndex + 2 );

    mesh.indices.push_back( startIndex + 0 );
    mesh.indices.push_back( startIndex + 2 );
    mesh.indices.push_back( startIndex + 3 );
};

// Méthode de création de Cube
Mesh MeshBuilder::CreateCube(float size) {
    Mesh mesh;
    float h = size * 0.5f; // demi-taille
    // Définir les 8 sommets du cube
    // Face avant (+Z)
    AddQuad(mesh,
        {-h, -h,  h},
        { h, -h,  h},
        { h,  h,  h},
        {-h,  h,  h},
        { 0,  0,  1}
    );
    // Face arrière (-Z)
    AddQuad(mesh,
        { h, -h, -h},
        {-h, -h, -h},
        {-h,  h, -h},
        { h,  h, -h},
        { 0,  0, -1}
    );
    // Face gauche (-X)
    AddQuad(mesh,
        {-h, -h, -h},
        {-h, -h,  h},
        {-h,  h,  h},
        {-h,  h, -h},
        {-1,  0,  0}
    );
    // Face droite (+X)
    AddQuad(mesh,
        { h, -h,  h},
        { h, -h, -h},
        { h,  h, -h},
        { h,  h,  h},
        { 1,  0,  0}
    );
    // Face haut (+Y)
    AddQuad(mesh,
        {-h,  h,  h},
        { h,  h,  h},
        { h,  h, -h},
        {-h,  h, -h},
        { 0,  1,  0}
    );
    // Face bas (-Y)
    AddQuad(mesh,
        {-h, -h, -h},
        { h, -h, -h},
        { h, -h,  h},
        {-h, -h,  h},
        { 0, -1,  0}
    );
    return mesh;    
}

// Méthodes de transformation de l'objet 3D
void Object3D::Translate(const Vector3& t) {
    transform.position += t;
}

// Méthodes de rotation de l'objet 3D
void Object3D::Rotate(const Vector3& r) {
    transform.rotation += r;
}

// Méthodes de mise à l'échelle de l'objet 3D
void Object3D::Scale(const Vector3& s) {
    transform.scale *= s;
}