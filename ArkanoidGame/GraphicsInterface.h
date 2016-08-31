#pragma once

#include "Types.h"
#include <vector>
#include "GameObject.h"
#include "UIText.h"

class GraphicsInterface {
public:
    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fFar, float _fNear, void * m_hwnd = nullptr) = 0;

    virtual bool run() = 0;
    virtual bool render(const std::vector<GameObject*> * _scene, const std::vector<UIText*> * _ui) = 0;
    virtual bool shutdown() = 0;
    virtual void setCamera(const Camera & _oCamera) = 0;
    virtual ~GraphicsInterface() {};
    virtual void * getHandle() = 0;
    static GraphicsInterface * getInstance() = delete;

protected:
    static GraphicsInterface * instance;
};