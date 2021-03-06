#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_math3d.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_glut_backend.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include "ogldev_texture.h"
#include "ogldev_lighting_technique.h"
#include "ogldev_mesh.h"
#include "ogldev_skybox.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_ASPECT ((WINDOW_WIDTH * 1.0) / (WINDOW_HEIGHT * 1.0))

class Application : public ICallbacks
{
private:

    float m_scale;
    LightingTechnique *m_pEffect;
    Camera *m_pCamera;
    DirectionalLight m_directionalLight;
    M3DPersProjInfo m_persProjInfo;
    Mesh *m_pMesh;
    SkyBox *m_pSkyBox;
    float m_matSpecularIntensity;
    float m_specularPower;

public:

    Application()
    {
        m_scale = 0.0f;

        m_pCamera = NULL;
        m_pEffect = NULL;
        m_pMesh = NULL;
        m_pSkyBox = NULL;

        m3dLoadPersProjInfo(m_persProjInfo, m3dDegToRad(60.0f), WINDOW_ASPECT, 0.1f, 10000.0f);

        m3dLoadVector3(m_directionalLight.direction, -1.0f, -1.0f, -1.0f);
        m_directionalLight.ambientIntensity = 0.3f;
        m3dCopyVector3(m_directionalLight.color, COLOR_WHITE);
        m_directionalLight.diffuseIntensity = 1.0f;

        m_matSpecularIntensity = 1.0f;
        m_specularPower = 32.0f;
    }

    ~Application()
    {
        delete m_pCamera;
        delete m_pMesh;
        delete m_pEffect;
        delete m_pSkyBox;
    }

    bool init()
    {
        M3DVector3f pos{ 0.0f, 0.0f, 1000.0f }, target{ 0.0f, 0.0f, -1.0f }, up{ 0.0f, 1.0f, 0.0f };

        m_pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

        //unsigned int indices[] = {
        //    0, 3, 1,
        //    1, 3, 2,
        //    2, 3, 0,
        //    0, 1, 2 };

        //createIndexBuffer(indices, sizeof(indices));
        //createVertexBuffer(indices, ARRAY_SIZE_IN_ELEMENTS(indices));

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->init())
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->enable();
        m_pEffect->setTextureUnit(0);

        m_pMesh = new Mesh();

        if (!m_pMesh->loadMesh("./content/earth.obj"))
        {
            printf("Error initializing the mesh\n");
            return false;
        }

        m_pSkyBox = new SkyBox(m_pCamera, m_persProjInfo);

        if (!m_pSkyBox->init("./content", "polluted_earth_ft.jpg", "polluted_earth_bk.jpg", "polluted_earth_up.jpg", "polluted_earth_dn.jpg", "polluted_earth_rt.jpg", "polluted_earth_lf.jpg"))
        {
            return false;
        }

        return true;
    }

    void run()
    {
        glutBackendRun(this);
    }

    virtual void renderSceneCB()
    {
        static float scale = 0.0f;
        scale += 0.05f;

        m_pCamera->onRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pSkyBox->render();

        Pipeline p;

        p.setCamera(*m_pCamera);
        p.setPerspectiveProj(m_persProjInfo);
        p.rotate(m3dDegToRad(scale), 0.0f, 1.0f, 0.0f);

        m_pEffect->enable();
        m_pEffect->setWVP(p.getWVPTrans());
        m_pEffect->setWorldMatrix(p.getWorldTrans());
        m_pEffect->setDirectionalLight(m_directionalLight);
        m_pEffect->setEyeWorldPos(m_pCamera->getPos());
        m_pEffect->setMatSpecularIntensity(m_matSpecularIntensity);
        m_pEffect->setSpecularPower(m_specularPower);

        m_pMesh->render();

        glutSwapBuffers();
    }

    virtual void keyboardCB(OGLDEV_KEY ogldevKey, OGLDEV_KEY_STATE state)
    {
        switch (ogldevKey)
        {
        case OGLDEV_KEY_ESCAPE:
        case OGLDEV_KEY_q:
            glutBackendLeaveMainLoop();
            break;
        case OGLDEV_KEY_a:
            if (m_directionalLight.ambientIntensity < 5.0f)
            {
                m_directionalLight.ambientIntensity += 0.05f;
            }
            break;
        case OGLDEV_KEY_s:
            if (m_directionalLight.ambientIntensity > 0.0f)
            {
                m_directionalLight.ambientIntensity -= 0.05f;
            }
            break;
        case OGLDEV_KEY_z:
            if (m_directionalLight.diffuseIntensity < 5.0f)
            {
                m_directionalLight.diffuseIntensity += 0.05f;
            }
            break;
        case OGLDEV_KEY_x:
            if (m_directionalLight.diffuseIntensity > 0.0f)
            {
                m_directionalLight.diffuseIntensity -= 0.05f;
            }
            break;
        default:
            m_pCamera->onKeyboard(ogldevKey);
            break;
        }
    }

    virtual void passiveMouseCB(int x, int y)
    {
        m_pCamera->onMouse(x, y);
    }

//private:
//
//    void calcNormals(const unsigned int* pIndices, unsigned int indexCount,
//        Vertex* pVertices, unsigned int vertexCount)
//    {
//        // Accumulate each triangle normal into each of the triangle vertices
//        for (unsigned int i = 0; i < indexCount; i += 3) {
//            unsigned int index0 = pIndices[i];
//            unsigned int index1 = pIndices[i + 1];
//            unsigned int index2 = pIndices[i + 2];
//            M3DVector3f v1, v2, normal;
//            m3dSubtractVectors3(v1, pVertices[index1].m_pos, pVertices[index0].m_pos);
//            m3dSubtractVectors3(v2, pVertices[index2].m_pos, pVertices[index0].m_pos);
//            m3dCrossProduct3(normal, v1, v2);
//            m3dNormalizeVector3(normal);
//
//            m3dAddVectors3(v1, pVertices[index0].m_normal, normal);
//            m3dCopyVector3(pVertices[index0].m_normal, v1);
//            m3dAddVectors3(v1, pVertices[index1].m_normal, normal);
//            m3dCopyVector3(pVertices[index1].m_normal, v1);
//            m3dAddVectors3(v1, pVertices[index2].m_normal, normal);
//            m3dCopyVector3(pVertices[index2].m_normal, v1);
//        }
//
//        // Normalize all the vertex normals
//        for (unsigned int i = 0; i < vertexCount; i++) {
//            m3dNormalizeVector3(pVertices[i].m_normal);
//        }
//    }
//
//    void createVertexBuffer(const unsigned int *pIndeces, unsigned int indexCount)
//    {
//        Vertex vertices[4] = {
//            Vertex(M3DVector3f{ -1.0f, -1.0f, 0.5773f }, M3DVector2f{ 0.0f, 0.0f }),
//            Vertex(M3DVector3f{ 0.0f, -1.0f, -1.15475f }, M3DVector2f{ 0.5f, 0.0f }),
//            Vertex(M3DVector3f{ 1.0f, -1.0f, 0.5773f }, M3DVector2f{ 1.0f, 0.0f }),
//            Vertex(M3DVector3f{ 0.0f, 1.0f, 0.0f }, M3DVector2f{ 0.5f, 1.0f })
//        };
//
//        unsigned int vertexCount = ARRAY_SIZE_IN_ELEMENTS(vertices);
//
//        calcNormals(pIndeces, indexCount, vertices, vertexCount);
//
//        glGenBuffers(1, &m_vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    }
//
//    void createIndexBuffer(const unsigned int *pIndeces, unsigned int sizeInBytes)
//    {
//        glGenBuffers(1, &m_ibo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndeces, GL_STATIC_DRAW);
//    }
};

int main(int argc, char **argv)
{
    glutBackendInit(argc, argv, false, false);

    if (!glutBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, true, "OpenGL Test"))
    {
        return 1;
    }

    Application *pApp = new Application();

    if (!pApp->init())
    {
        return 1;
    }

    pApp->run();

    delete pApp;

    return 0;
}