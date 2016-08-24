#include "OpenGLClass.h"

bool OpenGLClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd) {

    return true;
}



void OpenGLClass::setCamera(const Camera & _oCamera) {}

bool OpenGLClass::run() {

    return true;
}

bool OpenGLClass::render(const std::vector<GameObject*> * _scene) {

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
