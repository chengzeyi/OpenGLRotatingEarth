#include "ogldev_pipeline.h"

const M3DMatrix44f& Pipeline::getWorldTrans()
{
    M3DMatrix44f scaleTrans, rotateTrans, translationTrans, tmp;
    m3dScaleMatrix44(scaleTrans, m_scale);
    m3dRotationMatrix44(rotateTrans, m_rotateInfo);
    m3dTranslationMatrix44(translationTrans, m_worldPos);
    m3dMatrixMultiply44(tmp, translationTrans, rotateTrans);
    m3dMatrixMultiply44(m_wTransformation, tmp, scaleTrans);

    return m_wTransformation;
}

const M3DMatrix44f& Pipeline::getProjTrans()
{
    m3dMakePerspectiveMatrix(m_projTransformation, m_persProjInfo);
    return m_projTransformation;
}

const M3DMatrix44f& Pipeline::getWPTrans()
{
    getWorldTrans();
    getProjTrans();
    m3dMatrixMultiply44(m_wpTransformation, m_projTransformation, m_wTransformation);
    return m_wpTransformation;
}

const M3DMatrix44f& Pipeline::getViewTrans()
{
    M3DMatrix44f cameraTranslationTrans, cameraRotateTrans;
    m3dTranslationMatrix44(cameraTranslationTrans, -m_camera.pos[0], -m_camera.pos[1], -m_camera.pos[2]);
    m3dMakeCameraMatrix(cameraRotateTrans, m_camera.target, m_camera.up);

    m3dMatrixMultiply44(m_vTransformation, cameraRotateTrans, cameraTranslationTrans);
    return m_vTransformation;
}

const M3DMatrix44f& Pipeline::getWVTrans()
{
    getWorldTrans();
    getViewTrans();

    m3dMatrixMultiply44(m_wvTransformation, m_vTransformation, m_wTransformation);
    return m_wvTransformation;
}

const M3DMatrix44f& Pipeline::getVPTrans()
{
    getViewTrans();
    getProjTrans();

    m3dMatrixMultiply44(m_vpTransformation, m_projTransformation, m_vTransformation);
    return  m_vpTransformation;
}

const M3DMatrix44f& Pipeline::getWVPTrans()
{
    getWorldTrans();
    getVPTrans();

    m3dMatrixMultiply44(m_wvpTransformation, m_vpTransformation, m_wTransformation);
    return m_wvpTransformation;
}
