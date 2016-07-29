#pragma once

#include "InputManager.h"
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

class WindowsInputManager : public InputManager {
public:

    virtual Button getButtonPressed();
    virtual ButtonsStatus getButtonsPressed();
    static InputManager * getInstance();

protected:

    // MAP Button -> Key Val
    int Buttons[IM__BUTTONS_NUMBER] = {
        QUIT_KEY,
        PAUSE_KEY,
        ENTER_KEY,
        LEFT_KEY,
        RIGHT_KEY,
        UP_KEY,
        DOWN_KEY,
        NOTHING_KEY
    };
};