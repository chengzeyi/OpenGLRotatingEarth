#include <iostream>
#include "ogldev_texture.h"

Texture::Texture(GLenum textureTarget, const std::string& fileName)
{
    m_textureTarget = textureTarget;
    m_fileName = fileName;
}

bool Texture::load()
{
    try {
        m_image.read(m_fileName);
        m_image.write(&m_blob, "RGBA");
    }
    catch (Magick::Error& error) {
        std::cout << "Error loading texture '" << m_fileName << "': " << error.what() << std::endl;
        return false;
    }

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.columns(), m_image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_textureTarget, 0);

    return true;
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
