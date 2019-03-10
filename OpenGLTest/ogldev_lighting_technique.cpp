#include "ogldev_lighting_technique.h"
#include "ogldev_util.h"

const char *pLightingVSScriptFileName = "lighting.vert";
const char *pLightingFSScriptFileName = "lighting.frag";

LightingTechnique::LightingTechnique() {}

bool LightingTechnique::init()
{
    if (!Technique::init()) {
        return false;
    }

    if (!addShader(GL_VERTEX_SHADER, pLightingVSScriptFileName)) {
        return false;
    }

    if (!addShader(GL_FRAGMENT_SHADER, pLightingFSScriptFileName)) {
        return false;
    }

    if (!finalize()) {
        return false;
    }

    m_wvpLocation = getUniformLocation("gWVP");
    m_samplerLocation = getUniformLocation("gSampler");
    m_worldMatrixLocation = getUniformLocation("gWorld");
    m_dirLightLocation.color = getUniformLocation("gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity = getUniformLocation("gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity = getUniformLocation("gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction = getUniformLocation("gDirectionalLight.direction");
    m_eyeWorldPosLocation = getUniformLocation("gEyeWorldPos");
    m_matSpecularIntensityLocation = getUniformLocation("gMatSpecularIntensity");
    m_specularPowerLocation = getUniformLocation("gSpecularPower");
    m_numPointLightsLocation = getUniformLocation("gNumPointLights");
    m_numSpotLightsLocation = getUniformLocation("gNumSpotLights");

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++)
    {
        char name[128];

        SNPRINTF(name, sizeof(name), "gPointLights[%d].base.color", i);
        m_pointLightsLocation[i].color = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].base.ambientIntensity", i);
        m_pointLightsLocation[i].ambintIntensity = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].position", i);
        m_pointLightsLocation[i].position = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].base.diffuseIntensity", i);
        m_pointLightsLocation[i].diffuseIntensity = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].atten.constant", i);
        m_pointLightsLocation[i].atten.constant = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].atten.linear", i);
        m_pointLightsLocation[i].atten.linear = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gPointLights[%d].atten.quadratic", i);
        m_pointLightsLocation[i].atten.quadratic = getUniformLocation(name);

        if (m_pointLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].ambintIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].atten.constant == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].atten.linear == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].atten.quadratic == INVALID_UNIFORM_LOCATION)
        {
            return false;
        }
    }

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); i++)
    {
        char name[128];

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.base.color", i);
        m_spotLightsLocation[i].color = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.base.ambientIntensity", i);
        m_spotLightsLocation[i].ambintIntensity = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.position", i);
        m_spotLightsLocation[i].position = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.base.diffuseIntensity", i);
        m_spotLightsLocation[i].diffuseIntensity = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].direction", i);
        m_spotLightsLocation[i].direction = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].cosCutoff", i);
        m_spotLightsLocation[i].cosCutoff = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.atten.constant", i);
        m_spotLightsLocation[i].atten.constant = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.atten.linear", i);
        m_spotLightsLocation[i].atten.linear = getUniformLocation(name);

        SNPRINTF(name, sizeof(name), "gSpotLights[%d].base.atten.quadratic", i);
        m_spotLightsLocation[i].atten.quadratic = getUniformLocation(name);

        if (m_spotLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].ambintIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].direction == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].cosCutoff == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].atten.constant == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].atten.linear == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].atten.quadratic == INVALID_UNIFORM_LOCATION)
        {
            return false;
        }
    }

    return !(
        m_dirLightLocation.direction == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.color == INVALID_UNIFORM_LOCATION ||
        m_wvpLocation == INVALID_UNIFORM_LOCATION ||
        m_samplerLocation == INVALID_UNIFORM_LOCATION ||
        m_worldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        m_specularPowerLocation == INVALID_UNIFORM_LOCATION);
}

void LightingTechnique::setEyeWorldPos(const M3DVector3f& eyeWorldPos)
{
    glUniform3f(m_eyeWorldPosLocation, eyeWorldPos[0], eyeWorldPos[1], eyeWorldPos[2]);
}

void LightingTechnique::setMatSpecularIntensity(float intensity)
{
    glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void LightingTechnique::setSpecularPower(float power)
{
    glUniform1f(m_specularPowerLocation, power);
}

void LightingTechnique::setWVP(const M3DMatrix44f& wvp)
{
    glUniformMatrix4fv(m_wvpLocation, 1, GL_FALSE, (const GLfloat*)wvp);
}


void LightingTechnique::setTextureUnit(unsigned int textureUnit)
{
    glUniform1i(m_samplerLocation, textureUnit);
}


void LightingTechnique::setDirectionalLight(const DirectionalLight& light)
{
    glUniform3f(m_dirLightLocation.color, light.color[0], light.color[1], light.color[2]);
    glUniform1f(m_dirLightLocation.ambientIntensity, light.ambientIntensity);
    glUniform1f(m_dirLightLocation.diffuseIntensity, light.diffuseIntensity);
    M3DVector3f direction;
    m3dCopyVector3(direction, light.direction);
    m3dNormalizeVector3(direction);
    glUniform3f(m_dirLightLocation.direction, direction[0], direction[1], direction[2]);
}

void LightingTechnique::setPointLights(unsigned int numLights, const PointLight *pLights)
{
    glUniform1i(m_numPointLightsLocation, numLights);
    for (unsigned int i = 0; i < numLights; i++)
    {
        glUniform3f(m_pointLightsLocation[i].color, pLights[i].color[0], pLights[i].color[1], pLights[i].color[2]);
        glUniform1f(m_pointLightsLocation[i].ambintIntensity, pLights[i].ambientIntensity);
        glUniform1f(m_pointLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity);
        glUniform3f(m_pointLightsLocation[i].position, pLights[i].position[0], pLights[i].position[1], pLights[i].position[2]);
        glUniform1f(m_pointLightsLocation[i].atten.constant, pLights[i].attenuation.constant);
        glUniform1f(m_pointLightsLocation[i].atten.linear, pLights[i].attenuation.linear);
        glUniform1f(m_pointLightsLocation[i].atten.quadratic, pLights[i].attenuation.quadratic);
    }
}

void LightingTechnique::setSpotLights(unsigned numLights, const SpotLight* pLights)
{
    glUniform1i(m_numSpotLightsLocation, numLights);
    for (unsigned int i = 0; i < numLights; i++)
    {
        glUniform3f(m_spotLightsLocation[i].color, pLights[i].color[0], pLights[i].color[1], pLights[i].color[2]);
        glUniform1f(m_spotLightsLocation[i].ambintIntensity, pLights[i].ambientIntensity);
        glUniform1f(m_spotLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity);
        glUniform3f(m_spotLightsLocation[i].position, pLights[i].position[0], pLights[i].position[1], pLights[i].position[2]);
        glUniform3f(m_spotLightsLocation[i].direction, pLights[i].direction[0], pLights[i].direction[1], pLights[i].direction[2]);
        glUniform1f(m_spotLightsLocation[i].cosCutoff, pLights[i].cosCutoff);
        glUniform1f(m_spotLightsLocation[i].atten.constant, pLights[i].attenuation.constant);
        glUniform1f(m_spotLightsLocation[i].atten.linear, pLights[i].attenuation.linear);
        glUniform1f(m_spotLightsLocation[i].atten.quadratic, pLights[i].attenuation.quadratic);
    }
}

void LightingTechnique::setWorldMatrix(const M3DMatrix44f& worldInverse)
{
    glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_FALSE, (const GLfloat*)worldInverse);
}
