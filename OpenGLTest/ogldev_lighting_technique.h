#pragma once

#include "ogldev_technique.h"
#include "ogldev_math3d.h"
#include "ogldev_lights_common.h"

class LightingTechnique : public Technique
{
public:

    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;

    LightingTechnique();

    virtual bool init();

    void setEyeWorldPos(const M3DVector3f& eyeWorldPos);
    void setMatSpecularIntensity(float intensity);
    void setSpecularPower(float power);
    void setWVP(const M3DMatrix44f& wvp);
    void setTextureUnit(unsigned int textureUnit);
    void setDirectionalLight(const DirectionalLight& light);
    void setPointLights(unsigned int numLights, const PointLight *pLights);
    void setSpotLights(unsigned int numLights, const SpotLight *pLights);
    void setWorldMatrix(const M3DMatrix44f& worldInverse);

private:

    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_specularPowerLocation;

    GLuint m_wvpLocation;
    GLuint m_samplerLocation;
    GLuint m_worldMatrixLocation;
    GLuint m_numPointLightsLocation;
    GLuint m_numSpotLightsLocation;

    struct
    {
        GLuint color;
        GLuint ambientIntensity;
        GLuint direction;
        GLuint diffuseIntensity;
    } m_dirLightLocation;

    struct
    {
        GLuint color;
        GLuint ambintIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        struct
        {
            GLuint constant;
            GLuint linear;
            GLuint quadratic;
        } atten;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    struct
    {
        GLuint color;
        GLuint ambintIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        GLuint direction;
        GLuint cosCutoff;
        struct
        {
            GLuint constant;
            GLuint linear;
            GLuint quadratic;
        } atten;
    } m_spotLightsLocation[MAX_SPOT_LIGHTS];
};