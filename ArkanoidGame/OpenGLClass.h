#pragma once

#include "GraphicsInterface.h"

class OpenGLClass : public GraphicsInterface {
public:

    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd = nullptr);
    
    virtual void setCamera(const Camera & _oCamera);
    virtual bool run();
    virtual bool render();
    virtual bool shutdown();

    virtual void CameraGoUp() = delete;

    static GraphicsInterface * getInstance();
protected:
    OpenGLClass();
};
