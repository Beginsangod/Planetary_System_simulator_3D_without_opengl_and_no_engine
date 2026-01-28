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

    return S * Rx * Ry * Rz * T;
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

// Méthode de création de Cube
Mesh MeshBuilder::CreateCube(float size) {
    Mesh mesh;
    float halfSize = size * 0.5f;
    
    // 8 sommets d'un cube (SYMÉTRIQUE)
    Vector3 vertices[8] = {
        // Face avant (Z+)
        Vector3(-halfSize, -halfSize,  halfSize),  // 0: bas-gauche-avant
        Vector3( halfSize, -halfSize,  halfSize),  // 1: bas-droite-avant
        Vector3( halfSize,  halfSize,  halfSize),  // 2: haut-droite-avant
        Vector3(-halfSize,  halfSize,  halfSize),  // 3: haut-gauche-avant
        
        // Face arrière (Z-)
        Vector3(-halfSize, -halfSize, -halfSize),  // 4: bas-gauche-arrière
        Vector3( halfSize, -halfSize, -halfSize),  // 5: bas-droite-arrière
        Vector3( halfSize,  halfSize, -halfSize),  // 6: haut-droite-arrière
        Vector3(-halfSize,  halfSize, -halfSize)   // 7: haut-gauche-arrière
    };
    
    // Normales pour chaque face
    Vector3 normals[6] = {
        Vector3( 0,  0,  1),  // Face avant (Z+)
        Vector3( 0,  0, -1),  // Face arrière (Z-)
        Vector3( 1,  0,  0),  // Face droite (X+)
        Vector3(-1,  0,  0),  // Face gauche (X-)
        Vector3( 0,  1,  0),  // Face haut (Y+)
        Vector3( 0, -1,  0)   // Face bas (Y-)
    };
    
    // Couleurs différentes pour chaque face (pour débug visuel)
    Vector3 colors[6] = {
        Vector3(1.0f, 0.0f, 0.0f),  // Rouge - Face avant
        Vector3(0.0f, 1.0f, 0.0f),  // Vert - Face arrière
        Vector3(0.0f, 0.0f, 1.0f),  // Bleu - Face droite
        Vector3(1.0f, 1.0f, 0.0f),  // Jaune - Face gauche
        Vector3(1.0f, 0.0f, 1.0f),  // Magenta - Face haut
        Vector3(0.0f, 1.0f, 1.0f)   // Cyan - Face bas
    };
    
    // Construction des 6 faces (24 vertices au total, 4 par face)
    // Chaque face a sa propre normale
    
    // Face AVANT (Z+) - Rouge
    mesh.vertices.push_back({vertices[0], normals[0], colors[0]});  // 0
    mesh.vertices.push_back({vertices[1], normals[0], colors[0]});  // 1
    mesh.vertices.push_back({vertices[2], normals[0], colors[0]});  // 2
    mesh.vertices.push_back({vertices[3], normals[0], colors[0]});  // 3
    
    // Face ARRIÈRE (Z-) - Vert
    mesh.vertices.push_back({vertices[5], normals[1], colors[1]});  // 4
    mesh.vertices.push_back({vertices[4], normals[1], colors[1]});  // 5
    mesh.vertices.push_back({vertices[7], normals[1], colors[1]});  // 6
    mesh.vertices.push_back({vertices[6], normals[1], colors[1]});  // 7
    
    // Face DROITE (X+) - Bleu
    mesh.vertices.push_back({vertices[1], normals[2], colors[2]});  // 8
    mesh.vertices.push_back({vertices[5], normals[2], colors[2]});  // 9
    mesh.vertices.push_back({vertices[6], normals[2], colors[2]});  // 10
    mesh.vertices.push_back({vertices[2], normals[2], colors[2]});  // 11
    
    // Face GAUCHE (X-) - Jaune
    mesh.vertices.push_back({vertices[4], normals[3], colors[3]});  // 12
    mesh.vertices.push_back({vertices[0], normals[3], colors[3]});  // 13
    mesh.vertices.push_back({vertices[3], normals[3], colors[3]});  // 14
    mesh.vertices.push_back({vertices[7], normals[3], colors[3]});  // 15
    
    // Face HAUT (Y+) - Magenta
    mesh.vertices.push_back({vertices[3], normals[4], colors[4]});  // 16
    mesh.vertices.push_back({vertices[2], normals[4], colors[4]});  // 17
    mesh.vertices.push_back({vertices[6], normals[4], colors[4]});  // 18
    mesh.vertices.push_back({vertices[7], normals[4], colors[4]});  // 19
    
    // Face BAS (Y-) - Cyan
    mesh.vertices.push_back({vertices[4], normals[5], colors[5]});  // 20
    mesh.vertices.push_back({vertices[5], normals[5], colors[5]});  // 21
    mesh.vertices.push_back({vertices[1], normals[5], colors[5]});  // 22
    mesh.vertices.push_back({vertices[0], normals[5], colors[5]});  // 23
    
    // Indices pour les 12 triangles (2 par face, 6 faces)
    // Attention à l'ordre des vertices pour le culling !
    
    // Face avant (2 triangles)
    mesh.indices.push_back(0); mesh.indices.push_back(1); mesh.indices.push_back(2);
    mesh.indices.push_back(0); mesh.indices.push_back(2); mesh.indices.push_back(3);
    
    // Face arrière
    mesh.indices.push_back(4); mesh.indices.push_back(5); mesh.indices.push_back(6);
    mesh.indices.push_back(4); mesh.indices.push_back(6); mesh.indices.push_back(7);
    
    // Face droite
    mesh.indices.push_back(8); mesh.indices.push_back(9); mesh.indices.push_back(10);
    mesh.indices.push_back(8); mesh.indices.push_back(10); mesh.indices.push_back(11);
    
    // Face gauche
    mesh.indices.push_back(12); mesh.indices.push_back(13); mesh.indices.push_back(14);
    mesh.indices.push_back(12); mesh.indices.push_back(14); mesh.indices.push_back(15);
    
    // Face haut
    mesh.indices.push_back(16); mesh.indices.push_back(17); mesh.indices.push_back(18);
    mesh.indices.push_back(16); mesh.indices.push_back(18); mesh.indices.push_back(19);
    
    // Face bas
    mesh.indices.push_back(20); mesh.indices.push_back(21); mesh.indices.push_back(22);
    mesh.indices.push_back(20); mesh.indices.push_back(22); mesh.indices.push_back(23);
    
    return mesh;
}

// création mesh plane 
Mesh MeshBuilder::CreatePlane(float width, float height, int subdivisionsWidth, int subdivisionsHeight) {
    Mesh mesh;
    
    // Vérification des subdivisions minimales
    if (subdivisionsWidth < 1) subdivisionsWidth = 1;
    if (subdivisionsHeight < 1) subdivisionsHeight = 1;
    
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    
    // Calcul du pas de grille
    float stepX = width / subdivisionsWidth;
    float stepZ = height / subdivisionsHeight;
    
    // Génération des vertices
    for (int z = 0; z <= subdivisionsHeight; ++z) {
        for (int x = 0; x <= subdivisionsWidth; ++x) {
            float posX = -halfWidth + x * stepX;
            float posZ = -halfHeight + z * stepZ;
            
            // Alternance de couleurs pour effet damier
            float u = (float)x / subdivisionsWidth;
            float v = (float)z / subdivisionsHeight;
            bool isEven = ((x + z) % 2 == 0);
            Vector3 color = isEven ? Vector3(0.8f, 0.8f, 0.8f) : Vector3(0.6f, 0.6f, 0.6f);
            
            // Le plan est dans le plan XZ avec Y=0, normale pointant vers +Y
            mesh.vertices.push_back({
                Vector3(posX, 0.0f, posZ),
                Vector3(0.0f, 1.0f, 0.0f),  // Normale vers le haut
                color
            });
        }
    }
    
    // Génération des indices (2 triangles par carré)
    for (int z = 0; z < subdivisionsHeight; ++z) {
        for (int x = 0; x < subdivisionsWidth; ++x) {
            int topLeft = z * (subdivisionsWidth + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (subdivisionsWidth + 1) + x;
            int bottomRight = bottomLeft + 1;
            
            // Premier triangle (sens anti-horaire vu de dessus)
            mesh.indices.push_back(topLeft);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(topRight);
            
            // Deuxième triangle
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(bottomRight);
        }
    }
    
    return mesh;
}

// création sphere
Mesh MeshBuilder::CreateSphere(float radius, int sectorCount, int stackCount) {
    Mesh mesh;
    
    // Validation des paramètres
    if (sectorCount < 3) sectorCount = 3;
    if (stackCount < 2) stackCount = 2;
    
    const float PI = 3.14159265358979323846f;
    
    // Génération des vertices
    for (int stack = 0; stack <= stackCount; ++stack) {
        // Angle vertical (de 0 à PI, du pôle nord au pôle sud)
        float stackAngle = PI / 2.0f - stack * PI / stackCount;  // -PI/2 à PI/2
        float xy = radius * std::cos(stackAngle);    // r * cos(phi)
        float z = radius * std::sin(stackAngle);     // r * sin(phi)
        
        for (int sector = 0; sector <= sectorCount; ++sector) {
            // Angle horizontal (de 0 à 2PI)
            float sectorAngle = sector * 2.0f * PI / sectorCount;
            
            // Position du vertex
            float x = xy * std::cos(sectorAngle);
            float y = xy * std::sin(sectorAngle);
            Vector3 position(x, y, z);
            
            // Normale (pour une sphère centrée à l'origine, c'est la position normalisée)
            Vector3 normal = position.Normalisation();
            
            // Couleur basée sur la position (optionnel, pour debug visuel)
            float u = static_cast<float>(sector) / sectorCount;
            float v = static_cast<float>(stack) / stackCount;
            Vector3 color(0.8f + 0.2f * u, 0.8f + 0.2f * v, 1.0f);
            
            mesh.vertices.push_back({position, normal, color});
        }
    }
    
    // Génération des indices
    for (int stack = 0; stack < stackCount; ++stack) {
        int k1 = stack * (sectorCount + 1);      // Début de la stack actuelle
        int k2 = k1 + sectorCount + 1;           // Début de la stack suivante
        
        for (int sector = 0; sector < sectorCount; ++sector, ++k1, ++k2) {
            // Deux triangles par quad
            // k1 --- k1+1
            //  |  \   |
            // k2 --- k2+1
            
            // Premier triangle (k1, k2, k1+1)
            if (stack != 0) {  // Pas de triangle au pôle nord (dégénéré)
                mesh.indices.push_back(k1);
                mesh.indices.push_back(k2);
                mesh.indices.push_back(k1 + 1);
            }
            
            // Deuxième triangle (k1+1, k2, k2+1)
            if (stack != stackCount - 1) {  // Pas de triangle au pôle sud (dégénéré)
                mesh.indices.push_back(k1 + 1);
                mesh.indices.push_back(k2);
                mesh.indices.push_back(k2 + 1);
            }
        }
    }
    
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