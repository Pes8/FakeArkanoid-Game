#pragma once

#include "SystemFactory.h"

class WindowsSystemFactory : public SystemFactory {
public:
    virtual InputManager * getInputManager();
    virtual GraphicsManager * getGraphicsManager();
    virtual OSHelper * getOSHelper();
};