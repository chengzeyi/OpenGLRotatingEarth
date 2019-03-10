#include <stdio.h>

#include "ogldev_util.h"
#include "ogldev_technique.h"

Technique::Technique()
{
    m_shaderProg = 0;
}

Technique::~Technique()
{
    for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
    {
        glDeleteShader(*it);
    }

    if (m_shaderProg != 0)
    {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}

bool Technique::init()
{
    m_shaderProg = glCreateProgram();
    if (m_shaderProg == 0)
    {
        if (m_shaderProg == 0)
        {
            fprintf(stderr, "Error creating shader program\n");
            return false;
        }
    }

    return true;
}

bool Technique::addShader(GLenum shaderType, const char* pFileName)
{
    GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0)
    {
        fprintf(stderr, "Error creating shader type %d\n", shaderType);
        return false;
    }

    const GLchar* p[1];
    p[0] = readTextFile(pFileName);
    GLint lengths[1];
    lengths[0] = strlen(p[0]);
    glShaderSource(shaderObj, 1, p, lengths);
    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
        return false;
    }

    glAttachShader(m_shaderProg, shaderObj);

    return true;
}

bool Technique::finalize()
{
    GLint success;
    GLchar errorLog[1024];
    glLinkProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
        return false;
    }

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
        return false;
    }

    for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
    {
        glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return glCheckError();
}

void Technique::enable()
{
    glUseProgram(m_shaderProg);
}

GLint Technique::getUniformLocation(const char* pUniformName)
{
    GLuint location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (location == INVALID_UNIFORM_LOCATION)
    {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
}

GLint Technique::getProgramParam(GLint param)
{
    GLint ret;
    glGetProgramiv(m_shaderProg, param, &ret);
    return ret;
}



