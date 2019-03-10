#pragma once

#include "ogldev_math3d.h"
#include "ogldev_camera.h"

struct Orientation
{
    M3DVector3f m_scale;
    M3DVector4f m_rotation;
    M3DVector3f m_pos;

    Orientation()
    {
        m3dLoadVector3(m_scale, 1.0f, 1.0f, 1.0f);
        m3dLoadVector4(m_rotation, 0.0f, 0.0f, 0.0f, 0.0f);
        m3dLoadVector3(m_pos, 0.0f, 0.0f, 0.0f);
    }
};

class Pipeline
{
public:
    Pipeline()
    {
        m3dLoadVector3(m_scale, 1.0f, 1.0f, 1.0f);
        m3dLoadVector3(m_worldPos, 0.0f, 0.0f, 0.0f);
        m3dLoadVector4(m_rotateInfo, 0.0f, 0.0f, 0.0f, 0.0f);
    }

    void scale(float s)
    {
        scale(s, s, s);
    }


    void scale(const M3DVector3f& scale)
    {
        this->scale(scale[0], scale[1], scale[2]);
    }

    void scale(float scaleX, float scaleY, float scaleZ)
    {
        m3dLoadVector3(m_scale, scaleX, scaleY, scaleZ);
    }

    void worldPos(float x, float y, float z)
    {
        m3dLoadVector3(m_worldPos, x, y, z);
    }

    void worldPos(const M3DVector3f& pos)
    {
        m3dCopyVector3(m_worldPos, pos);
    }

    void rotate(float angle, float x, float y, float z)
    {
        m3dLoadVector4(m_rotateInfo, angle, x, y, z);
    }

    void rotate(const M3DVector4f& r)
    {
        rotate(r[0], r[1], r[2], r[3]);
    }

    void setPerspectiveProj(const M3DPersProjInfo& p)
    {
        m_persProjInfo = p;
    }

    void setCamera(const M3DVector3f& pos, const M3DVector3f& target, const M3DVector3f& up)
    {
        m3dCopyVector3(m_camera.pos, pos);
        m3dCopyVector3(m_camera.target, target);
        m3dCopyVector3(m_camera.up, up);
    }

    void setCamera(const Camera& camera)
    {
        setCamera(camera.getPos(), camera.getTarget(), camera.getUp());
    }

    const M3DMatrix44f& getWorldTrans();
    const M3DMatrix44f& getProjTrans();
    const M3DMatrix44f& getWPTrans();
    const M3DMatrix44f& getViewTrans();
    const M3DMatrix44f& getWVTrans();
    const M3DMatrix44f& getVPTrans();
    const M3DMatrix44f& getWVPTrans();

private:
    M3DVector3f m_scale;
    M3DVector3f m_worldPos;
    M3DVector4f m_rotateInfo;

    M3DPersProjInfo m_persProjInfo;
    M3DOrthoProjInfo m_orthoProjInfo;

    struct {
        M3DVector3f pos;
        M3DVector3f target;
        M3DVector3f up;
    } m_camera;

    M3DMatrix44f m_wvpTransformation;
    M3DMatrix44f m_vpTransformation;
    M3DMatrix44f m_wpTransformation;
    M3DMatrix44f m_wvTransformation;
    M3DMatrix44f m_wTransformation;
    M3DMatrix44f m_vTransformation;
    M3DMatrix44f m_projTransformation;
};
