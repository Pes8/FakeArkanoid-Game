#pragma once

#include "GraphicsInterface.h"

class OpenGLClass : public GraphicsInterface {
public:

    virtual bool initialize(unsigned int_iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd = nullptr);
    
    virtual void setCamera(const Camera & _oCamera);
    virtual bool run();
    virtual bool render(const std::vector<GameObject*> * _scene, const std::vector<UIText*> * _ui);
    virtual bool shutdown();

    virtual void * getHandle();

    virtual void CameraGoUp() = delete;

    static GraphicsInterface * getInstance();
protected:
    OpenGLClass();
};
