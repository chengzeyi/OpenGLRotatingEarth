#pragma once

#include <string>
#include <GL/glew.h>
#include "Magick++.h"

using namespace std;

class CubemapTexture
{
public:
    CubemapTexture(
        const string& directory,
        const string& posXFileName,
        const string& negXFileName,
        const string& posYFileName,
        const string& negYFileName,
        const string& posZFileName,
        const string& negZFileName
    );

    CubemapTexture();

    bool load();

    void bind(GLenum textureUnit);

private:
    string m_fileNames[6];
    GLuint m_textureObj;
};