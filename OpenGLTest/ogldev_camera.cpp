#include "ogldev_camera.h"

const static float STEP_SCALE = 5.00f;
const static float EDGE_STEP = 0.05f;
const static int MARGIN = 100;

Camera::Camera(int windowWidth, int windowHeight)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m3dLoadVector3(m_pos, 0.0f, 0.0f, 0.0f);
    m3dLoadVector3(m_target, 0.0f, 0.0f, 1.0f);
    m3dNormalizeVector3(m_target);
    m3dLoadVector3(m_up, 0.0f, 1.0f, 0.0f);

    init();
}

Camera::Camera(int windowWidth, int windowHeight, const M3DVector3f& pos, const M3DVector3f& target, const M3DVector3f& up)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m3dCopyVector3(m_pos, pos);

    m3dCopyVector3(m_target, target);
    m3dNormalizeVector3(m_target);

    m3dCopyVector3(m_up, up);
    m3dNormalizeVector3(m_up);

    init();
}

bool Camera::onKeyboard(OGLDEV_KEY key)
{
    bool ret = false;

    switch (key)
    {
    case OGLDEV_KEY_UP:
        {
            M3DVector3f tmpA, tmpB;
            m3dCopyVector3(tmpA, m_target);
            m3dScaleVector3(tmpA, STEP_SCALE);
            m3dAddVectors3(tmpB, m_pos, tmpA);
            m3dCopyVector3(m_pos, tmpB);
            ret = true;
        }
        break;
    case OGLDEV_KEY_DOWN:
        {
            M3DVector3f tmpA, tmpB;
            m3dCopyVector3(tmpA, m_target);
            m3dScaleVector3(tmpA, STEP_SCALE);
            m3dSubtractVectors3(tmpB, m_pos, tmpA);
            m3dCopyVector3(m_pos, tmpB);
            ret = true;
        }
        break;
    case OGLDEV_KEY_LEFT:
        {
            M3DVector3f left, tmp;
            m3dCrossProduct3(left, m_target, m_up);
            m3dNormalizeVector3(left);
            m3dScaleVector3(left, STEP_SCALE);
            m3dAddVectors3(tmp, m_pos, left);
            m3dCopyVector3(m_pos, tmp);
            ret = true;
        }
        break;
    case OGLDEV_KEY_RIGHT:
        {
            M3DVector3f right, tmp;
            m3dCrossProduct3(right, m_up, m_target);
            m3dNormalizeVector3(right);
            m3dScaleVector3(right, STEP_SCALE);
            m3dAddVectors3(tmp, m_pos, right);
            m3dCopyVector3(m_pos, tmp);
            ret = true;
        }
        break;
    case OGLDEV_KEY_PAGE_UP:
        m_pos[1] += STEP_SCALE;
        break;
    case OGLDEV_KEY_PAGE_DOWN:
        m_pos[1] -= STEP_SCALE;
        break;
    }

    return ret;
}

void Camera::onMouse(int x, int y)
{
    const int deltaX = x - m_mousePos[0];
    const int deltaY = y - m_mousePos[1];


    m_mousePos[0] = x;
    m_mousePos[1] = y;

    m_angleH += deltaX / 20.0f;
    m_angleV += deltaY / 20.0f;

    if (deltaX == 0)
    {
        if (x <= MARGIN)
        {
            m_onLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN))
        {
            m_onRightEdge = true;
        }
    }
    else
    {
        m_onLeftEdge = false;
        m_onRightEdge = false;
    }

    if (deltaY == 0)
    {
        if (y <= MARGIN)
        {
            m_onUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN))
        {
            m_onLowerEdge = true;
        }
    }
    else
    {
        m_onUpperEdge = false;
        m_onLowerEdge = false;
    }

    update();
}

void Camera::onRender()
{
    bool shouldUpdate = false;

    if (m_onLeftEdge)
    {
        m_angleH -= EDGE_STEP;
        shouldUpdate = true;
    }
    else if (m_onRightEdge)
    {
        m_angleH += EDGE_STEP;
        shouldUpdate = true;
    }

    if (m_onUpperEdge)
    {
        if (m_angleV > -90.f)
        {
            m_angleV -= EDGE_STEP;
            shouldUpdate = true;
        }
    }
    else if (m_onLowerEdge)
    {
        if (m_angleV < 90.0f)
        {
            m_angleV += EDGE_STEP;
            shouldUpdate = true;
        }
    }

    if (shouldUpdate)
    {
        update();
    }
}

void Camera::init()
{
    M3DVector3f hTarget;
    m3dLoadVector3(hTarget, m_target[0], 0.0f, m_target[2]);
    m3dNormalizeVector3(hTarget);

    if (hTarget[2] >= 0.0f)
    {
        if (hTarget[0] >= 0.0f)
        {
            m_angleH = 360.0f - m3dRadToDeg(asin(hTarget[2]));
        }
        else
        {
            m_angleH = 180.0f + m3dRadToDeg(asin(hTarget[2]));
        }
    }
    else
    {
        if (hTarget[0] >= 0.0f)
        {
            m_angleH = m3dRadToDeg(asin(-hTarget[2]));
        }
        else
        {
            m_angleH = 180.0f - m3dRadToDeg(asin(-hTarget[2]));
        }
    }

    m_angleV = -m3dRadToDeg(asin(m_target[1]));

    m_onUpperEdge = false;
    m_onLowerEdge = false;
    m_onLeftEdge = false;
    m_onRightEdge = false;
    m_mousePos[0] = m_windowWidth / 2;
    m_mousePos[1] = m_windowHeight / 2;

    // glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

void Camera::update()
{
    const M3DVector4f vAxis{0.0f, 1.0f, 0.0f };

    M3DVector3f tmp{ 1.0f, 0.0f, 0.0f }, view;
    M3DMatrix33f rotateMatrix;
    m3dRotationMatrix33(rotateMatrix, m3dDegToRad(m_angleH), vAxis[0], vAxis[1], vAxis[2]);
    m3dRotateVector(view, tmp, rotateMatrix);
    m3dNormalizeVector3(view);

    M3DVector3f hAxis;
    m3dCrossProduct3(hAxis, vAxis, view);
    m3dNormalizeVector3(hAxis);
    m3dRotationMatrix33(rotateMatrix, m3dDegToRad(m_angleV), hAxis[0], hAxis[1], hAxis[2]);
    m3dRotateVector(tmp, view, rotateMatrix);

    m3dCopyVector3(m_target, tmp);
    m3dNormalizeVector3(m_target);

    m3dCrossProduct3(m_up, m_target, hAxis);
    m3dNormalizeVector3(m_up);
}
