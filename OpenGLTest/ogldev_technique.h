#pragma once

#include <list>
#include <GL/glew.h>

class Technique
{
public:
    Technique();
    virtual ~Technique();
    virtual bool init();
    void enable();

protected:
    bool addShader(GLenum shaderType, const char *pFileName);
    bool finalize();
    GLint getUniformLocation(const char *pUniformName);
    GLint getProgramParam(GLint param);
    GLuint m_shaderProg;

private:
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
};