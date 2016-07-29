#pragma once

#include "InputManager.h"
#include "GraphicsManager.h"
#include "OSHelper.h"

class SystemFactory {
public:
    virtual InputManager * getInputManager() = 0;
    virtual GraphicsManager * getGraphicsManager() = 0;
    virtual OSHelper * getOSHelper() = 0;
};