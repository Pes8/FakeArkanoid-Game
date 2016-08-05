#pragma once

#include "Types.h"
#include "Block.h"

class GraphicsInterface {
public:
    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd = nullptr) = 0;
    virtual bool loadAsset(Block * _block) = 0;
    virtual bool run() = 0;
    virtual bool render() = 0;
    virtual bool shutdown() = 0;

    virtual ~GraphicsInterface() {};

    static GraphicsInterface * getInstance() = delete;

protected:
    static GraphicsInterface * instance;
};