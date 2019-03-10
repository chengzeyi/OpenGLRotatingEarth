#include <string>

#include "ogldev_skybox_technique.h"
#include "ogldev_util.h"

SkyboxTechnique::SkyboxTechnique() {}

bool SkyboxTechnique::init()
{
    if (!Technique::init())
    {
        return false;
    }

    if (!addShader(GL_VERTEX_SHADER, "skybox.vert"))
    {
        return false;
    }

    if (!addShader(GL_FRAGMENT_SHADER, "skybox.frag"))
    {
        return false;
    }

    if (!finalize())
    {
        return false;
    }

    m_wvpLocation = getUniformLocation("gWVP");
    m_textureLocation = getUniformLocation("gCubemapTexture");

    return !(m_wvpLocation == INVALID_UNIFORM_LOCATION || m_textureLocation == INVALID_UNIFORM_LOCATION);
}

void SkyboxTechnique::setWVP(const M3DMatrix44f& wvp)
{
    glUniformMatrix4fv(m_wvpLocation, 1, GL_FALSE, (const GLfloat *)wvp);
}

void SkyboxTechnique::setTextureUnit(unsigned int textureUnit)
{
    glUniform1i(m_textureLocation, textureUnit);
}