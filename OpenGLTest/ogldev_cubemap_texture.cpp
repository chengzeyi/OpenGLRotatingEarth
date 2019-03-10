#include <iostream>
#include "ogldev_cubemap_texture.h"
#include "ogldev_util.h"

static const GLenum types[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

CubemapTexture::CubemapTexture(const string& directory, const string& posXFileName, const string& negXFileName,
    const string& posYFileName, const string& negYFileName, const string& posZFileName, const string& negZFileName)
{
    auto iter = directory.crend();
    string baseDir = (*iter == '/') ? directory : directory + "/";
    m_fileNames[0] = baseDir + posXFileName;
    m_fileNames[1] = baseDir + negXFileName;
    m_fileNames[2] = baseDir + posYFileName;
    m_fileNames[3] = baseDir + negYFileName;
    m_fileNames[4] = baseDir + posZFileName;
    m_fileNames[5] = baseDir + negZFileName;
    m_textureObj = 0;
}

CubemapTexture::CubemapTexture()
{
    if (m_textureObj != 0)
    {
        glDeleteTextures(1, &m_textureObj);
    }
}

bool CubemapTexture::load()
{
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

    Magick::Image *pImage = NULL;
    Magick::Blob blob;

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++)
    {
        pImage = new Magick::Image(m_fileNames[i]);
        try
        {
            pImage->write(&blob, "RGBA");
        }
        catch (Magick::Error &error)
        {
            cout << "Error loading texture '" << m_fileNames[i] << "': " << error.what() << endl;
            delete pImage;
            return false;
        }

        glTexImage2D(types[i], 0, GL_RGB, pImage->columns(), pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        delete pImage;
    }

    return true;
}

void CubemapTexture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}
