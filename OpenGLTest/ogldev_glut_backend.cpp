#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_util.h"
#include "ogldev_glut_backend.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static ICallbacks* s_pCallbacks = NULL;

static bool sWithDepth = false;
static bool sWithStencil = false;

OGLDEV_KEY glutKeyToOgldevKey(uint key)
{
    switch (key) {
    case GLUT_KEY_F1:
        return OGLDEV_KEY_F1;
    case GLUT_KEY_F2:
        return OGLDEV_KEY_F2;
    case GLUT_KEY_F3:
        return OGLDEV_KEY_F3;
    case GLUT_KEY_F4:
        return OGLDEV_KEY_F4;
    case GLUT_KEY_F5:
        return OGLDEV_KEY_F5;
    case GLUT_KEY_F6:
        return OGLDEV_KEY_F6;
    case GLUT_KEY_F7:
        return OGLDEV_KEY_F7;
    case GLUT_KEY_F8:
        return OGLDEV_KEY_F8;
    case GLUT_KEY_F9:
        return OGLDEV_KEY_F9;
    case GLUT_KEY_F10:
        return OGLDEV_KEY_F10;
    case GLUT_KEY_F11:
        return OGLDEV_KEY_F11;
    case GLUT_KEY_F12:
        return OGLDEV_KEY_F12;
    case GLUT_KEY_LEFT:
        return OGLDEV_KEY_LEFT;
    case GLUT_KEY_UP:
        return OGLDEV_KEY_UP;
    case GLUT_KEY_RIGHT:
        return OGLDEV_KEY_RIGHT;
    case GLUT_KEY_DOWN:
        return OGLDEV_KEY_DOWN;
    case GLUT_KEY_PAGE_UP:
        return OGLDEV_KEY_PAGE_UP;
    case GLUT_KEY_PAGE_DOWN:
        return OGLDEV_KEY_PAGE_DOWN;
    case GLUT_KEY_HOME:
        return OGLDEV_KEY_HOME;
    case GLUT_KEY_END:
        return OGLDEV_KEY_END;
    case GLUT_KEY_INSERT:
        return OGLDEV_KEY_INSERT;
    case GLUT_KEY_DELETE:
        return OGLDEV_KEY_DELETE;
    default:
        return OGLDEV_KEY_UNDEFINED;
        // OGLDEV_ERROR0("Unimplemented GLUT key");
        // exit(1);
    }

    return OGLDEV_KEY_UNDEFINED;
}


static OGLDEV_MOUSE glutMouseToOgldevMouse(uint button)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        return OGLDEV_MOUSE_BUTTON_LEFT;
    case GLUT_RIGHT_BUTTON:
        return OGLDEV_MOUSE_BUTTON_RIGHT;
    case GLUT_MIDDLE_BUTTON:
        return OGLDEV_MOUSE_BUTTON_MIDDLE;
    default:
        //OGLDEV_ERROR0("Unimplemented GLUT mouse button");
        break;
    }

    return OGLDEV_MOUSE_UNDEFINED;
}


static void specialKeyboardCB(int key, int x, int y)
{
    OGLDEV_KEY ogldevKey = glutKeyToOgldevKey(key);
    s_pCallbacks->keyboardCB(ogldevKey);
}


static void keyboardCB(unsigned char key, int x, int y)
{

    if (
        ((key >= '+') && (key <= '9')) ||
        ((key >= 'A') && (key <= 'Z')) ||
        ((key >= 'a') && (key <= 'z'))
        ) {
        OGLDEV_KEY ogldevKey = (OGLDEV_KEY)key;
        s_pCallbacks->keyboardCB(ogldevKey);
    }
    else {
        //printf("%d\n", key);
        //OGLDEV_ERROR0("Unimplemented GLUT key");
    }

}


static void passiveMouseCB(int x, int y)
{
    s_pCallbacks->passiveMouseCB(x, y);
}


static void renderSceneCB()
{
    s_pCallbacks->renderSceneCB();
}


static void idleCB()
{
    s_pCallbacks->renderSceneCB();
}


static void mouseCB(int button, int state, int x, int y)
{
    OGLDEV_MOUSE ogldevMouse = glutMouseToOgldevMouse(button);
    OGLDEV_KEY_STATE ogldevKeyState = (state == GLUT_DOWN) ? OGLDEV_KEY_STATE_PRESS : OGLDEV_KEY_STATE_RELEASE;

    s_pCallbacks->mouseCB(ogldevMouse, ogldevKeyState, x, y);
}


static void initCallbacks()
{
    glutDisplayFunc(renderSceneCB);
    glutIdleFunc(idleCB);
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
}


void glutBackendInit(int argc, char** argv, bool withDepth, bool withStencil)
{
    sWithDepth = withDepth;
    sWithStencil = withStencil;

    glutInit(&argc, argv);

    uint displayMode = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH;

    if (withDepth) {
        displayMode |= GLUT_DEPTH;
    }

    if (withStencil) {
        displayMode |= GLUT_STENCIL;
    }

    glutInitDisplayMode(displayMode);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}


bool glutBackendCreateWindow(uint width, uint height, bool isFullScreen, const char* pTitle)
{
    if (isFullScreen) {
        char modeString[64] = { 0 };
        SNPRINTF(modeString, sizeof(modeString), "%dx%d", width, height);
        glutGameModeString(modeString);
        glutEnterGameMode();
    }
    else {
        glutInitWindowSize(width, height);
        glutCreateWindow(pTitle);
    }

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }

    return true;
}

void glutBackendRun(ICallbacks* pCallbacks)
{
    if (!pCallbacks) {
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (sWithDepth) {
        glEnable(GL_DEPTH_TEST);
    }

    s_pCallbacks = pCallbacks;
    initCallbacks();
    glutMainLoop();
}


void glutBackendSwapBuffers()
{
    glutSwapBuffers();
}

void glutBackendLeaveMainLoop()
{
    glutLeaveMainLoop();
}