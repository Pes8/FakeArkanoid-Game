#pragma once

#include "Types.h"
#include <vector>
#include "GenericAsset.h"
#include "Scene.h"

class GraphicsInterface {
public:
    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fFar, float _fNear, void * m_hwnd = nullptr) = 0;

    virtual bool run() = 0;
    virtual bool render(Scene * _scene) = 0;
    virtual bool shutdown() = 0;
    virtual void setCamera(const Camera & _oCamera) = 0;
    virtual ~GraphicsInterface() {};

    static GraphicsInterface * getInstance() = delete;

protected:
    static GraphicsInterface * instance;
};