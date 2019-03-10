#pragma once

#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>

#include "ogldev_types.h"

using namespace std;

char * readTextFile(const char *pFileName);

void ogldevError(const char* pFileName, uint line, const char* msg, ...);
void ogldevFileError(const char* pFileName, uint line, const char* pFileError);

#define OGLDEV_ERROR0(msg) ogldevError(__FILE__, __LINE__, msg)
#define OGLDEV_ERROR(msg, ...) ogldevError(__FILE__, __LINE__, msg, __VA_ARGS__)
#define OGLDEV_FILE_ERROR(fileError) ogldevFileError(__FILE__, __LINE__, fileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifdef WIN32
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define glExitIfError                                                           \
{                                                                               \
    GLenum error = glGetError();                                                \
                                                                                \
    if (error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define glCheckError() (glGetError() == GL_NO_ERROR)

long long getCurrentTimeMillis();

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
