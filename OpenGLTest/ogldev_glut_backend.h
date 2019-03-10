#pragma once

#include "ogldev_types.h"
#include "ogldev_callbacks.h"
#include "ogldev_keys.h"

void glutBackendInit(int argc, char** argv, bool withDepth, bool withStencil);

bool glutBackendCreateWindow(uint width, uint height, bool isFullScreen, const char* pTitle);

void glutBackendRun(ICallbacks* pCallbacks);

void glutBackendSwapBuffers();

void glutBackendLeaveMainLoop();

OGLDEV_KEY glutKeyToOgldevKey(uint key);
