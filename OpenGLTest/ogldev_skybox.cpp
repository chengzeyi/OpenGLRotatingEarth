#include "ogldev_skybox.h"
#include "ogldev_pipeline.h"
#include "ogldev_util.h"

SkyBox::SkyBox(const Camera* pCamera, const M3DPersProjInfo& p)
{
    m_pCamera = pCamera;
    m_persProjInfo = p;
    m_pCubemapTexture = NULL;
    m_pSkyBoxTechnique = NULL;
    m_pMesh = NULL;
}

SkyBox::~SkyBox()
{
    SAFE_DELETE(m_pCubemapTexture);
    SAFE_DELETE(m_pSkyBoxTechnique);
    SAFE_DELETE(m_pMesh);
}

bool SkyBox::init(const string& directory, const string& posXFileName, const string& negXFileName,
    const string& posYFileName, const string& negYFileName, const string& posZFileName, const string& negZFileName)
{
    m_pSkyBoxTechnique = new SkyboxTechnique();

    if (!m_pSkyBoxTechnique->init())
    {
        printf("Error initializing the skybox technique\n");
        return false;
    }

    m_pSkyBoxTechnique->enable();
    m_pSkyBoxTechnique->setTextureUnit(0);

    m_pCubemapTexture = new CubemapTexture(
        directory,
        posXFileName,
        negXFileName,
        posYFileName,
        negYFileName,
        posZFileName,
        negZFileName
    );

    if (!m_pCubemapTexture->load())
    {
        return false;
    }

    m_pMesh = new Mesh();

    return m_pMesh->loadMesh("./content/sphere.obj");
}

void SkyBox::render()
{
    m_pSkyBoxTechnique->enable();

    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    GLint oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    Pipeline p;
    p.scale(100.0f, 100.0f, 100.0f);
    p.worldPos(m_pCamera->getPos());
    p.setCamera(*m_pCamera);
    p.setPerspectiveProj(m_persProjInfo);
    m_pSkyBoxTechnique->setWVP(p.getWVPTrans());
    m_pCubemapTexture->bind(GL_TEXTURE0);
    m_pMesh->render();

    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFuncMode);
}
