#pragma once

#include "ogldev_math3d.h"
#include "ogldev_keys.h"

class Camera
{
public:

    Camera(int windowWidth, int windowHeight);

    Camera(int windowWidth, int windowHeight, const M3DVector3f& pos, const M3DVector3f& target, const M3DVector3f& up);

    bool onKeyboard(OGLDEV_KEY key);

    void onMouse(int x, int y);

    void onRender();

    const M3DVector3f& getPos() const
    {
        return m_pos;
    }

    const M3DVector3f& getTarget() const
    {
        return m_target;
    }

    const M3DVector3f& getUp() const
    {
        return m_up;
    }

private:

    void init();
    void update();

    M3DVector3f m_pos;
    M3DVector3f m_target;
    M3DVector3f m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_angleH;
    float m_angleV;

    bool m_onUpperEdge;
    bool m_onLowerEdge;
    bool m_onLeftEdge;
    bool m_onRightEdge;

    M3DVector2i m_mousePos;
};
