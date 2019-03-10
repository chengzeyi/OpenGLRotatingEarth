#pragma once

#include "ogldev_camera.h"
#include "ogldev_skybox_technique.h"
#include "ogldev_cubemap_texture.h"
#include "ogldev_mesh.h"

class SkyBox
{
public:
    SkyBox(const Camera *pCamera, const const M3DPersProjInfo &p);

    ~SkyBox();

    bool init(
        const string& directory,
        const string& posXFileName,
        const string& negXFileName,
        const string& posYFileName,
        const string& negYFileName,
        const string& posZFileName,
        const string& negZFileName
    );

    void render();

private:
    SkyboxTechnique *m_pSkyBoxTechnique;
    const Camera *m_pCamera;
    CubemapTexture *m_pCubemapTexture;
    Mesh *m_pMesh;
    M3DPersProjInfo m_persProjInfo;
};