#include "OpenGLClass.h"

bool OpenGLClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd) {

    return true;
}

bool OpenGLClass::loadAsset(Block * _block) {
    return false;
}

bool OpenGLClass::run() {

    return true;
}

bool OpenGLClass::render() {

    return true;
}

bool OpenGLClass::shutdown() {
    return true;
}

GraphicsInterface * OpenGLClass::getInstance() {
    if (OpenGLClass::instance == nullptr) {
        OpenGLClass::instance = new OpenGLClass();
    }
    return OpenGLClass::instance;
}

OpenGLClass::OpenGLClass() {}
