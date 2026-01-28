#pragma once
#include <SDL3/SDL.h>
#include "scene.h"

// Énumération pour les modes de rendu
enum class RenderMode {
    FILLED,      // Rendu normal avec remplissage
    WIREFRAME,   // Seulement les arêtes
    POINTS       // Seulement les points
};

// Classe en charge du rendu 3D
class Renderer3d {
    private:
        int width;
        int height;
        Vector3 clearcolor;
        std::vector<uint32_t> framebuffer;
        std::vector<float> depthbuffer;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        RenderMode renderMode;  // Mode de rendu actuel

    public:
        Renderer3d(int w, int h, SDL_Renderer* renderer) : width(w), height(h), renderer(renderer), renderMode(RenderMode::FILLED) { 
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        }
        ~Renderer3d(){if(texture) SDL_DestroyTexture(texture);};

        // Gestion du mode de rendu
        void SetRenderMode(RenderMode mode) { renderMode = mode; }
        RenderMode GetRenderMode() const { return renderMode; }

        /* pipelines de rendu */
        void InitializeBuffers();
        void Clear(const Vector3& color);
        void RenderScene(Scene scene);
        void DrawObject(const Object3D& obj, Matrice4 Model, Matrice4 View, Matrice4 Proj);
        void DrawMesh(const Mesh& mesh, const Matrice4& Model, const Matrice4& View, const Matrice4& Proj);  
        void TransformVertex(const Vertex& inVertex, VertexTransformed& outVertex, const Matrice4& MVP, const Matrice4& MV);     
        float Trianglecheck(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        void RasterizeTriangle(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, bool isFrontFacing);
        void BoundBox(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, int& minX, int& minY, int& maxX, int& maxY);
        Vector3 BarycentricCoordinates(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        bool isInTriangle(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        bool DepthTest(Vector2 screenCoord, float depth);
        void WritePixel(Vector2 screenCoord, const Vector3& color);
        void Present();

        // Nouvelles méthodes pour le rendu wireframe et points
        void DrawLine(const VertexTransformed& v0, const VertexTransformed& v1, const Vector3& color);
        void DrawPoint(const VertexTransformed& v, const Vector3& color, int size = 3);
};