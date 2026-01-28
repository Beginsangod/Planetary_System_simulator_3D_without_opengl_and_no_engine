#include "Renderer3d.h"
#include <algorithm>

using namespace std;

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
        
        DrawObject(object, modelMatrix, viewMatrix, projectionMatrix);
    }
}

// Dessin d'un objet 3D spécifique en utilisant la matrice MVP
void Renderer3d::DrawObject(const Object3D& obj, Matrice4 Model, Matrice4 View, Matrice4 Proj) {
    DrawMesh(obj.mesh, Model, View, Proj);
}

// Dessin d'un mesh en transformant et rasterisant ses triangles
void Renderer3d::DrawMesh(const Mesh& mesh, const Matrice4& Model, const Matrice4& View, const Matrice4& Proj) {
    // Calculer MVP UNE SEULE FOIS pour tout le mesh
    Matrice4 MV = Model * View;
    Matrice4 MVP = MV * Proj;
    
    // Transformer tous les vertices avec la même matrice
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        VertexTransformed v0, v1, v2; 
        TransformVertex(mesh.vertices[mesh.indices[i]], v0, MVP, MV);
        TransformVertex(mesh.vertices[mesh.indices[i + 1]], v1, MVP, MV);
        TransformVertex(mesh.vertices[mesh.indices[i + 2]], v2, MVP, MV);
        
        if (v0.valid && v1.valid && v2.valid) {
            float area = Trianglecheck(v0, v1, v2);
            if (std::abs(area) > 0.0001f) {
                switch (renderMode) {
                    case RenderMode::FILLED:
                        RasterizeTriangle(v0, v1, v2, area > 0);
                        break;
                    
                    case RenderMode::WIREFRAME:
                        DrawLine(v0, v1, Vector3(1.0f, 1.0f, 1.0f));
                        DrawLine(v1, v2, Vector3(1.0f, 1.0f, 1.0f));
                        DrawLine(v2, v0, Vector3(1.0f, 1.0f, 1.0f));
                        break;
                    
                    case RenderMode::POINTS:
                        DrawPoint(v0, Vector3(1.0f, 0.0f, 0.0f), 5);
                        DrawPoint(v1, Vector3(0.0f, 1.0f, 0.0f), 5);
                        DrawPoint(v2, Vector3(0.0f, 0.0f, 1.0f), 5);
                        break;
                }
            }
        }
    }
}

void Renderer3d::TransformVertex(const Vertex& inVertex, VertexTransformed& outVertex, const Matrice4& MVP, const Matrice4& MV) {
    // ROW-MAJOR: point * MVP
    point clipPos = point(inVertex.position) * MVP;
    
    // Stocker la position de clipping
    outVertex.clipPosition = clipPos;
    
    // Vérifier si w est valide
    if (clipPos.w <= 0.0f) {
        outVertex.valid = false;
        return;
    }
    
    // Frustum clipping
    float margin = 0.1f;
    if (clipPos.x < -clipPos.w - margin || clipPos.x > clipPos.w + margin || clipPos.y < -clipPos.w - margin || clipPos.y > clipPos.w + margin || clipPos.z < -clipPos.w - margin || clipPos.z > clipPos.w + margin) {
        outVertex.valid = false;
        return;
    }
    
    // Perspective divide
    outVertex.invW = 1.0f / clipPos.w;
    
    // NDC to Screen
    outVertex.screenCoord.x = (clipPos.x * outVertex.invW + 1.0f) * 0.5f * width;
    outVertex.screenCoord.y = (1.0f - clipPos.y * outVertex.invW) * 0.5f * height;
    outVertex.depth = clipPos.z * outVertex.invW;
    
    // Couleur
    outVertex.color = inVertex.color;
    
    // Normale en espace vue
    outVertex.normal_view = MV.TransformDirection(inVertex.normal).Normalisation();
    outVertex.normal_over_w = outVertex.normal_view * outVertex.invW;
    
    // Le vertex est valide
    outVertex.valid = true;
}

float Renderer3d::Trianglecheck(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    return (v1.screenCoord.x - v0.screenCoord.x) * (v2.screenCoord.y - v0.screenCoord.y) -
           (v2.screenCoord.x - v0.screenCoord.x) * (v1.screenCoord.y - v0.screenCoord.y);
}

void Renderer3d::RasterizeTriangle(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, bool isFrontFacing) {
    if (isFrontFacing) {
        return;
    }

    int minX, minY, maxX, maxY;
    BoundBox(v0, v1, v2, minX, minY, maxX, maxY);

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            Vector2 pixel(x + 0.5f, y + 0.5f);

            if (isInTriangle(pixel, v0, v1, v2)) {
                Vector3 bary = BarycentricCoordinates(pixel, v0, v1, v2);
                float depth = bary.x * v0.depth + bary.y * v1.depth + bary.z * v2.depth;

                if (DepthTest(pixel, depth)) {
                    Vector3 colorOverW = (v0.color * v0.invW) * bary.x +
                                        (v1.color * v1.invW) * bary.y +
                                        (v2.color * v2.invW) * bary.z;

                    float invW = v0.invW * bary.x + v1.invW * bary.y + v2.invW * bary.z;
                    Vector3 color = colorOverW * (1.0f / invW);

                    Vector3 normalOverW = v0.normal_over_w * bary.x +
                                         v1.normal_over_w * bary.y +
                                         v2.normal_over_w * bary.z;

                    Vector3 normal = (normalOverW * (1.0f / invW)).Normalisation();

                    Vector3 lightDir = Vector3(0.0f, 0.0f, -1.0f).Normalisation();
                    float intensity = std::max(0.2f, -normal.Dot(lightDir));

                    Vector3 finalColor = color * intensity;

                    WritePixel(pixel, finalColor);
                }
            }
        }
    }
}

void Renderer3d::BoundBox(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, int& minX, int& minY, int& maxX, int& maxY) {
    minX = std::floor(std::min({ static_cast<int>(v0.screenCoord.x), static_cast<int>(v1.screenCoord.x), static_cast<int>(v2.screenCoord.x) }));
    minY = std::floor(std::min({ static_cast<int>(v0.screenCoord.y), static_cast<int>(v1.screenCoord.y), static_cast<int>(v2.screenCoord.y) }));
    maxX = std::ceil(std::max({ static_cast<int>(v0.screenCoord.x), static_cast<int>(v1.screenCoord.x), static_cast<int>(v2.screenCoord.x) }));
    maxY = std::ceil(std::max({ static_cast<int>(v0.screenCoord.y), static_cast<int>(v1.screenCoord.y), static_cast<int>(v2.screenCoord.y) }));

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, width - 1);
    maxY = std::min(maxY, height - 1);
}

bool Renderer3d::isInTriangle(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    Vector3 bary = BarycentricCoordinates(p, v0, v1, v2);
    return (bary.x >= 0) && (bary.y >= 0) && (bary.z >= 0);
}

inline Vector3 Renderer3d::BarycentricCoordinates(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2) {
    const float dx1 = v1.screenCoord.x - v0.screenCoord.x;
    const float dy1 = v1.screenCoord.y - v0.screenCoord.y;
    const float dx2 = v2.screenCoord.x - v0.screenCoord.x;
    const float dy2 = v2.screenCoord.y - v0.screenCoord.y;
    const float dxp = p.x - v0.screenCoord.x;
    const float dyp = p.y - v0.screenCoord.y;

    const float d00 = dx1 * dx1 + dy1 * dy1;
    const float d01 = dx1 * dx2 + dy1 * dy2;
    const float d11 = dx2 * dx2 + dy2 * dy2;
    const float d20 = dxp * dx1 + dyp * dy1;
    const float d21 = dxp * dx2 + dyp * dy2;

    const float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    
    const float v = (d11 * d20 - d01 * d21) * invDenom;
    const float w = (d00 * d21 - d01 * d20) * invDenom;
    const float u = 1.0f - v - w;

    return Vector3(u, v, w);
}

bool Renderer3d::DepthTest(Vector2 screenCoord, float depth) {
    int x = static_cast<int>(screenCoord.x);
    int y = static_cast<int>(screenCoord.y);
    
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    
    int index = y * width + x;
    
    if (index < 0 || index >= static_cast<int>(depthbuffer.size())) {
        return false;
    }
    
    if (depth < depthbuffer[index]) {
        depthbuffer[index] = depth;
        return true;
    }
    return false;
}

inline void Renderer3d::WritePixel(Vector2 screenCoord, const Vector3& color)
{
    int x = (int)screenCoord.x;
    int y = (int)screenCoord.y;

    if (x < 0 || y < 0 || x >= width || y >= height)
        return;

    Uint8 r = (Uint8)(std::clamp(color.x, 0.0f, 1.0f) * 255.0f);
    Uint8 g = (Uint8)(std::clamp(color.y, 0.0f, 1.0f) * 255.0f);
    Uint8 b = (Uint8)(std::clamp(color.z, 0.0f, 1.0f) * 255.0f);

    int index = y * width + x;
    
    if (index < 0 || index >= static_cast<int>(framebuffer.size()))
        return;

    framebuffer[index] =
        (255 << 24) |
        (r   << 16) |
        (g   << 8)  |
        (b);
}

void Renderer3d::DrawLine(const VertexTransformed& v0, const VertexTransformed& v1, const Vector3& color) {
    int x0 = static_cast<int>(v0.screenCoord.x);
    int y0 = static_cast<int>(v0.screenCoord.y);
    int x1 = static_cast<int>(v1.screenCoord.x);
    int y1 = static_cast<int>(v1.screenCoord.y);
    
    if ((x0 < -100 && x1 < -100) || (x0 > width + 100 && x1 > width + 100) ||
        (y0 < -100 && y1 < -100) || (y0 > height + 100 && y1 > height + 100)) {
        return;
    }
    
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int maxIterations = width + height;
    int iterations = 0;
    
    while (iterations++ < maxIterations) {
        float t = 0.0f;
        float totalDist = std::sqrt(static_cast<float>((x1 - static_cast<int>(v0.screenCoord.x)) * (x1 - static_cast<int>(v0.screenCoord.x)) + 
                                                       (y1 - static_cast<int>(v0.screenCoord.y)) * (y1 - static_cast<int>(v0.screenCoord.y))));
        if (totalDist > 0.0f) {
            float currentDist = std::sqrt(static_cast<float>((x0 - static_cast<int>(v0.screenCoord.x)) * (x0 - static_cast<int>(v0.screenCoord.x)) + 
                                                             (y0 - static_cast<int>(v0.screenCoord.y)) * (y0 - static_cast<int>(v0.screenCoord.y))));
            t = currentDist / totalDist;
        }
        float depth = v0.depth * (1.0f - t) + v1.depth * t;
        
        Vector2 p(static_cast<float>(x0), static_cast<float>(y0));
        
        if (DepthTest(p, depth)) {
            WritePixel(p, color);
        }
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Renderer3d::DrawPoint(const VertexTransformed& v, const Vector3& color, int size) {
    int cx = static_cast<int>(v.screenCoord.x);
    int cy = static_cast<int>(v.screenCoord.y);
    
    if (cx < -size || cy < -size || cx >= width + size || cy >= height + size) {
        return;
    }
    
    int halfSize = size / 2;
    
    for (int dy = -halfSize; dy <= halfSize; ++dy) {
        for (int dx = -halfSize; dx <= halfSize; ++dx) {
            int px = cx + dx;
            int py = cy + dy;
            
            if (px >= 0 && py >= 0 && px < width && py < height) {
                Vector2 p(static_cast<float>(px), static_cast<float>(py));
                if (DepthTest(p, v.depth)) {
                    WritePixel(p, color);
                }
            }
        }
    }
}

void Renderer3d::Present() {
    SDL_UpdateTexture( texture, nullptr, framebuffer.data(), width * 4 );
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}