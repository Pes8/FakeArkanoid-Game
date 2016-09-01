#pragma once

#include "GraphicsInterface.h"
#include "Types.h"

#ifdef WINDOWS 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // WINDOWS

#include <gl\GL.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

class OpenGLClass : public GraphicsInterface {
public:

    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd = nullptr);
    
    virtual void setCamera(const Camera & _oCamera);
    virtual bool run();
    virtual bool render(const std::vector<GameObject*> * _scene, const std::vector<UIText*> * _ui);
    virtual bool shutdown();

    virtual void * getHandle();

    virtual void CameraGoUp() = delete;

    static GraphicsInterface * getInstance();
protected:
    OpenGLClass();
    void ReSizeGLScene(unsigned int _iScreenWidth, unsigned int _iScreenHeight);
    bool loadMesh(GameObject * _object);
    bool loadTexture(Texture * _tex);
#ifdef WINDOWS
    HDC m_hdc; // device context handle
#endif
};
