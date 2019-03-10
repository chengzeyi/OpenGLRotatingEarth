#pragma once

#include "ogldev_keys.h"

class ICallbacks
{
public:

    virtual void keyboardCB(OGLDEV_KEY ogldevKey, OGLDEV_KEY_STATE ogldevKeyState = OGLDEV_KEY_STATE_PRESS) {};

    virtual void passiveMouseCB(int x, int y) {};

    virtual void renderSceneCB() {};

    virtual void mouseCB(OGLDEV_MOUSE button, OGLDEV_KEY_STATE state, int x, int y) {};
};
