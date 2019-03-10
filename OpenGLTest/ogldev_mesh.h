#pragma once

#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ogldev_util.h"
#include "ogldev_math3d.h"
#include "ogldev_texture.h"

struct Vertex
{
    M3DVector3f m_pos;
    M3DVector2f m_tex;
    M3DVector3f m_normal;

    Vertex() {}

    Vertex(const M3DVector3f& pos, const M3DVector2f& tex, const M3DVector3f& normal)
    {
        m3dCopyVector3(m_pos, pos);
        m3dCopyVector2(m_tex, tex);
        m3dCopyVector3(m_normal, normal);
    }
};

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool loadMesh(const std::string& filename);

    void render();

private:
    bool initFromScene(const aiScene* pScene, const std::string& filename);
    void initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* pScene, const std::string& filename);
    void clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        void init(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices);

        GLuint vb;
        GLuint ib;
        unsigned int numIndices;
        unsigned int materialIndex;
    };

    std::vector<MeshEntry> m_entries;
    std::vector<Texture *> m_textures;
};
