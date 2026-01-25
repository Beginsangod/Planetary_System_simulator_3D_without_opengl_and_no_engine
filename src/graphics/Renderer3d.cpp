#include "Renderer3d.h"
#include <algorithm>

// Initialisation des buffers de rendu(en charge de stocker les pixels et la profondeur)
void Renderer3d::InitializeBuffers() {
    framebuffer.resize(width * height, 0);
    depthbuffer.resize(width * height, std::numeric_limits<float>::infinity());
}

// Nettoyage des buffers avec une couleur spécifique et reinitialisation du depthbuffer
void Renderer3d::Clear(const Vector3& color) {
    clearcolor = color;
    uint32_t packedColor = (255 << 24) |
                           (static_cast<uint8_t>(color.x * 255) << 16) |
                           (static_cast<uint8_t>(color.y * 255) << 8) |
                           (static_cast<uint8_t>(color.z * 255));
    std::fill(framebuffer.begin(), framebuffer.end(), packedColor);
    std::fill(depthbuffer.begin(), depthbuffer.end(), std::numeric_limits<float>::infinity());
}

// Rendu de la scène 3D complète
void Renderer3d::RenderScene(Scene scene) {
    Camera cam = scene._Camera();

    Matrice4 viewMatrix = cam.GetViewMatrix();
    Matrice4 projectionMatrix = cam.GetProjectionMatrix();

    for (const Object3D& object : scene.getObjects()) {
        Matrice4 modelMatrix = object.transform.GetMatrix();
        
        // DEBUG: Affiche les matrices complètes
        SDL_Log("=== DEBUG Model Matrix ===");
        for(int i=0; i<4; i++) {
            SDL_Log("[%.2f %.2f %.2f %.2f]", 
                modelMatrix.mat[i][0], modelMatrix.mat[i][1], 
                modelMatrix.mat[i][2], modelMatrix.mat[i][3]);
        }
        
        DrawObject(object, modelMatrix, viewMatrix, projectionMatrix);
    }
}

// Dessin d'un objet 3D spécifique en utilisant la matrice MVP
void Renderer3d::DrawObject(const Object3D& obj, Matrice4 Model, Matrice4 View, Matrice4 Proj) {
    DrawMesh(obj.mesh, Model, View, Proj);
}

// Dessin d'un mesh en transformant et rasterisant ses triangles
void Renderer3d::DrawMesh(const Mesh& mesh, Matrice4 Model, Matrice4 View, Matrice4 Proj) {
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        VertexTransformed v0, v1, v2;
        TransformVertex(mesh.vertices[mesh.indices[i]], v0, Model, View, Proj);
        TransformVertex(mesh.vertices[mesh.indices[i + 1]], v1, Model, View, Proj);
        TransformVertex(mesh.vertices[mesh.indices[i + 2]], v2, Model, View, Proj);
        
        if (v0.valid && v1.valid && v2.valid && Trianglecheck(v0, v1, v2))
        {
            RasterizeTriangle(v0, v1, v2);
        }
    }
}

void Renderer3d::TransformVertex(const Vertex& inVertex, VertexTransformed& outVertex,
                                 Matrice4 Model, Matrice4 View, Matrice4 Proj)
{
    // 1. Model space → World space
    point worldPos = point(inVertex.position) * Model;
    
    // 2. World space → View space  
    point viewPos = worldPos * View;
    
    SDL_Log("VIEW = %.2f %.2f %.2f %.2f", viewPos.x, viewPos.y, viewPos.z, viewPos.w);
    
    // 3. View space → Clip space
    point clipPos = viewPos * Proj;
    
    SDL_Log("CLIP = %.2f %.2f %.2f %.2f", clipPos.x, clipPos.y, clipPos.z, clipPos.w);
    
    // VÉRIFICATION CRITIQUE
    if (clipPos.w < -0.1f) {  // Accepte les W négatifs (objets derrière)
        outVertex.valid = false;
        return;
    }

    // Pour les W négatifs, inverse le signe
    if(clipPos.w < 0) {
        clipPos.w = -clipPos.w;
        clipPos.x = -clipPos.x;
        clipPos.y = -clipPos.y;
        clipPos.z = -clipPos.z;
    }
    
    outVertex.invW = 1.0f / clipPos.w;
    
    // 4. Clip → NDC
    float ndcX = clipPos.x * outVertex.invW;
    float ndcY = clipPos.y * outVertex.invW;
    float ndcZ = clipPos.z * outVertex.invW;
    
    // 5. NDC → Screen
    outVertex.screenCoord.x = (ndcX + 1.0f) * 0.5f * width;
    outVertex.screenCoord.y = (1.0f - ndcY) * 0.5f * height;
    outVertex.depth = ndcZ;
    
    // 6. Couleur et normale
    outVertex.color = inVertex.color;
    outVertex.normal_view = View.TransformDirection(inVertex.normal).Normalisation();
    outVertex.normal_over_w = outVertex.normal_view * outVertex.invW;
    
    outVertex.valid = true;
}


bool Renderer3d::Trianglecheck( const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2)
{
    float area =
        (v1.screenCoord.x - v0.screenCoord.x) * (v2.screenCoord.y - v0.screenCoord.y) -
        (v1.screenCoord.y - v0.screenCoord.y) * (v2.screenCoord.x - v0.screenCoord.x);

    // Triangle dégénéré (aire ~ 0)
    if (std::abs(area) < 0.0001f)
        return false;

    return true;
}


void Renderer3d::RasterizeTriangle(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    SDL_Log("RasterizeTriangle called");
    int minX, minY, maxX, maxY;
    BoundBox(v0, v1, v2, minX, minY, maxX, maxY);
    
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vector2 p(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
            // test barycentriques
            if (isInTriangle(p, v0, v1, v2)) {
                Vector3 bary = BarycentricCoordinates(p, v0, v1, v2);
                
                // Interpolation perspective-correct
                float z_over_w = bary.x * (v0.depth * v0.invW) + bary.y * (v1.depth * v1.invW) + bary.z * (v2.depth * v2.invW);
                float inv_w = bary.x * v0.invW + bary.y * v1.invW + bary.z * v2.invW;
                float depth = z_over_w / inv_w;
                Vector3 normal_over_w = v0.normal_over_w * bary.x + v1.normal_over_w * bary.y + v2.normal_over_w * bary.z;

                if (DepthTest(p, depth)) {
                    Vector3 normal = (normal_over_w / inv_w).Normalisation();
                    Vector3 lightDir = Vector3(0, 0, -1).Normalisation();
                    float intensity = std::max(0.0f, normal.Dot(lightDir));
                    Vector3 color = Vector3(1.0f, 1.0f, 1.0f) * intensity;
                    WritePixel(p, color);
                }
            }
        }
    }
}

// Calcul de la boite englobante 2D d'un triangle
void Renderer3d::BoundBox(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, int& minX, int& minY, int& maxX, int& maxY) {
    // coordonnées de la boite englobante min et max 2d du triangle arrondies
    minX = std::floor(std::min({ static_cast<int>(v0.screenCoord.x), static_cast<int>(v1.screenCoord.x), static_cast<int>(v2.screenCoord.x) }));
    minY = std::floor(std::min({ static_cast<int>(v0.screenCoord.y), static_cast<int>(v1.screenCoord.y), static_cast<int>(v2.screenCoord.y) }));
    maxX = std::ceil(std::max({ static_cast<int>(v0.screenCoord.x), static_cast<int>(v1.screenCoord.x), static_cast<int>(v2.screenCoord.x) }));
    maxY = std::ceil(std::max({ static_cast<int>(v0.screenCoord.y), static_cast<int>(v1.screenCoord.y), static_cast<int>(v2.screenCoord.y) }));

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, width - 1);
    maxY = std::min(maxY, height - 1);
    SDL_Log("BBox: min(%d,%d) max(%d,%d)", minX, minY, maxX, maxY);
}

bool Renderer3d::isInTriangle(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    Vector3 bary = BarycentricCoordinates(p, v0, v1, v2);
    return (bary.x >= 0) && (bary.y >= 0) && (bary.z >= 0);
}

Vector3 Renderer3d::BarycentricCoordinates(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    Vector2 v0v1 = v1.screenCoord - v0.screenCoord;
    Vector2 v0v2 = v2.screenCoord - v0.screenCoord;
    Vector2 v0p = p - v0.screenCoord;

    float d00 = v0v1.Dot(v0v1);
    float d01 = v0v1.Dot(v0v2);
    float d11 = v0v2.Dot(v0v2);
    float d20 = v0p.Dot(v0v1);
    float d21 = v0p.Dot(v0v2);

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return Vector3(u, v, w);
}

bool Renderer3d::DepthTest(Vector2 screenCoord, float depth) {
    int index = static_cast<int>(screenCoord.y) * width + static_cast<int>(screenCoord.x);
    if (depth < depthbuffer[index]) {
        depthbuffer[index] = depth;
        return true;
    }
    return false;
}

void Renderer3d::WritePixel(Vector2 screenCoord, const Vector3& color)
{
    SDL_Log("write pixel enter");
    int x = (int)screenCoord.x;
    int y = (int)screenCoord.y;

    // Sécurité
    if (x < 0 || y < 0 || x >= width || y >= height)
        return;

    // Clamp couleur
    Uint8 r = (Uint8)(std::clamp(color.x, 0.0f, 1.0f) * 255.0f);
    Uint8 g = (Uint8)(std::clamp(color.y, 0.0f, 1.0f) * 255.0f);
    Uint8 b = (Uint8)(std::clamp(color.z, 0.0f, 1.0f) * 255.0f);

    // ARGB8888 : A R G B (8 bits chacun)
    framebuffer[y * width + x] =
        (255 << 24) |  // Alpha
        (r   << 16) |  // Red
        (g   << 8)  |  // Green
        (b);

    static int count = 0;
    count++;
    if (count < 10)
        SDL_Log("WritePixel called (%d)", count);
}



void Renderer3d::Present() {
    SDL_UpdateTexture( texture, nullptr, framebuffer.data(), width * 4 );
    //SDL_Log("Framebuffer[0]=0x%08X ", framebuffer[0]);
    //SDL_Log("Framebuffer[mid]=0x%08X ", framebuffer[(height/2)*width + width/2]);
    //SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t));
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}