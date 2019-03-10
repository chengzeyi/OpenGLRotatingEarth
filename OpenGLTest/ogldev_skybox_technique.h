#pragma once

#include "ogldev_math3d.h"
#include "ogldev_technique.h"


class SkyboxTechnique : public Technique {
public:
    SkyboxTechnique();

    virtual bool init();

    void setWVP(const M3DMatrix44f& wvp);
    void setTextureUnit(unsigned int textureUnit);

private:
    GLuint m_wvpLocation;
    GLuint m_textureLocation;
};
