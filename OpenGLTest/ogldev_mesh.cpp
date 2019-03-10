#include "ogldev_mesh.h"

Mesh::MeshEntry::MeshEntry()
{
    vb = INVALID_OGL_VALUE;
    ib = INVALID_OGL_VALUE;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
    if (vb != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &vb);
    }

    if (ib != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &ib);
    }
}

void Mesh::MeshEntry::init(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    numIndices = indices.size();

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
    clear();
}


void Mesh::clear()
{
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        SAFE_DELETE(m_textures[i]);
    }
}


bool Mesh::loadMesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    bool ret = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);

    if (pScene) {
        ret = initFromScene(pScene, filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
    }

    return ret;
}

bool Mesh::initFromScene(const aiScene* pScene, const std::string& filename)
{
    m_entries.resize(pScene->mNumMeshes);
    m_textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_entries.size(); i++) {
        const aiMesh* pAIMesh = pScene->mMeshes[i];
        initMesh(i, pAIMesh);
    }

    return initMaterials(pScene, filename);
}

void Mesh::initMesh(unsigned int index, const aiMesh* pAIMesh)
{
    m_entries[index].materialIndex = pAIMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < pAIMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(pAIMesh->mVertices[i]);
        const aiVector3D* pNormal = &(pAIMesh->mNormals[i]);
        const aiVector3D* pTexCoord = pAIMesh->HasTextureCoords(0) ? &(pAIMesh->mTextureCoords[0][i]) : &zero3D;

        Vertex v(M3DVector3f{ pPos->x, pPos->y, pPos->z },
            M3DVector2f{ pTexCoord->x, pTexCoord->y },
            M3DVector3f{ pNormal->x, pNormal->y, pNormal->z });

        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < pAIMesh->mNumFaces; i++) {
        const aiFace& face = pAIMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    m_entries[index].init(vertices, indices);
}

bool Mesh::initMaterials(const aiScene* pScene, const std::string& filename)
{
    // Extract the directory part from the file name
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos) {
        dir = ".";
    }
    else if (slashIndex == 0) {
        dir = "/";
    }
    else {
        dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string fullPath = dir + "/" + path.data;
                m_textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

                if (!m_textures[i]->load()) {
                    printf("Error loading texture '%s'\n", fullPath.c_str());
                    delete m_textures[i];
                    m_textures[i] = NULL;
                    ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", fullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!m_textures[i]) {
            m_textures[i] = new Texture(GL_TEXTURE_2D, "./content/white.png");

            ret = m_textures[i]->load();
        }
    }

    return ret;
}

void Mesh::render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0; i < m_entries.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_entries[i].vb);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, m_pos));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, m_tex));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, m_normal));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entries[i].ib);

        const unsigned int materialIndex = m_entries[i].materialIndex;

        if (materialIndex < m_textures.size() && m_textures[materialIndex]) {
            m_textures[materialIndex]->bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_entries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
