#pragma once

#ifdef WINDOWS

#include <Windows.h>


enum ButtonKey {
    QUIT_KEY = VK_ESCAPE,
    PAUSE_KEY = 0x50,
    ENTER_KEY = VK_RETURN,
    LEFT_KEY = VK_LEFT,
    RIGHT_KEY = VK_RIGHT,
    UP_KEY = VK_UP,
    DOWN_KEY = VK_DOWN,
    NOTHING_KEY = -666
};

#endif