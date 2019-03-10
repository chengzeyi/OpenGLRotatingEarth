#pragma once

#include "ogldev_math3d.h"

class BaseLight
{
public:
    std::string name;
    M3DVector3f color;
    float ambientIntensity;
    float diffuseIntensity;

    BaseLight()
    {
        m3dLoadVector3(color, 0.0f, 0.0f, 0.0f);
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
    }
};


class DirectionalLight : public BaseLight
{
public:
    M3DVector3f direction;

    DirectionalLight()
    {
        m3dLoadVector3(direction, 0.0f, 0.0f, 0.0f);
    }
};


struct LightAttenuation
{
    float constant;
    float linear;
    float quadratic;

    LightAttenuation()
    {
        constant = 1.0f;
        linear = 0.0f;
        quadratic = 0.0f;
    }
};


class PointLight : public BaseLight
{
public:
    M3DVector3f position;
    LightAttenuation attenuation;

    PointLight()
    {
        m3dLoadVector3(position, 0.0f, 0.0f, 0.0f);
    }
};


class SpotLight : public PointLight
{
public:
    M3DVector3f direction;
    float cosCutoff;

    SpotLight()
    {
        m3dLoadVector3(direction, 0.0f, 0.0f, 0.0f);
        cosCutoff = 0.0f;
    }
};


#define COLOR_WHITE M3DVector3f{ 1.0f, 1.0f, 1.0f }
#define COLOR_RED M3DVector3f{ 1.0f, 0.0f, 1.0f }
#define COLOR_GREEN M3DVector3f{ 0.0f, 1.0f, 0.0f }
#define COLOR_CYAN M3DVector3f{ 0.0f, 1.0f, 1.0f }
#define COLOR_BLUE M3DVector3f{ 0.0f, 0.0f, 1.0f }
