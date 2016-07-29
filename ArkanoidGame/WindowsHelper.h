#pragma once

#include "OSHelper.h"
#include <Windows.h>

class WindowsHelper : public OSHelper {
public:
    virtual void sleep_uS(unsigned long _microSeconds);
    static OSHelper * getInstance();
};