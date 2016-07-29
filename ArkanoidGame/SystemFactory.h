#pragma once

#include "InputManager.h"
#include "GraphicsManager.h"

class SystemFactory {
public:
    virtual InputManager * getInputManager() = 0;
    virtual GraphicsManager * getGraphicsManager() = 0;
};