#pragma once
#include <SDL3/SDL.h>
#include "scene.h"

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

    public:
        Renderer3d(int w, int h, SDL_Renderer* renderer) : width(w), height(h), renderer(renderer) { 
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        }
        ~Renderer3d(){if(texture) SDL_DestroyTexture(texture);};

        /* pipielines de rendu */
        void InitializeBuffers();
        void Clear(const Vector3& color);
        void RenderScene(Scene scene);
        void DrawObject(const Object3D& obj, Matrice4 Model, Matrice4 View, Matrice4 Proj);
        void DrawMesh(const Mesh& mesh, Matrice4 Model, Matrice4 View, Matrice4 Proj);
        void TransformVertex(const Vertex& inVertex, VertexTransformed& outVertex, Matrice4 Model, Matrice4 View, Matrice4 Proj);
        bool Trianglecheck(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        void RasterizeTriangle(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        void BoundBox(const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2, int& minX, int& minY, int& maxX, int& maxY);
        Vector3 BarycentricCoordinates(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        bool isInTriangle(const Vector2& p, const VertexTransformed& v0, const VertexTransformed& v1, const VertexTransformed& v2);
        bool DepthTest(Vector2 screenCoord, float depth);
        void WritePixel(Vector2 screenCoord, const Vector3& color);
        void Present();
};