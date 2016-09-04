#pragma once

#include "InputManager.h"
#include <Windows.h>

enum ButtonKey : unsigned char {
    QUIT_KEY = VK_ESCAPE,
    PAUSE_KEY = 0x50,
    ENTER_KEY = VK_RETURN,
    LEFT_KEY = VK_LEFT,
    RIGHT_KEY = VK_RIGHT,
    UP_KEY = VK_UP,
    DOWN_KEY = VK_DOWN,
    N_KEY = 0x4E,
    D0_KEY = 0x30,
    D1_KEY = 0x31,
    D2_KEY = 0x32,
    D3_KEY = 0x33,
    D4_KEY = 0x34,
    D5_KEY = 0x35,
    D6_KEY = 0x36,
    D7_KEY = 0x37,
    D8_KEY = 0x38,
    D9_KEY = 0x39,
    NOTHING_KEY = 0
};

class WindowsInputManager : public InputManager {
public:

    virtual Button getButtonPressed();
    virtual ButtonsStatus getButtonsPressed();
    static InputManager * getInstance();
    virtual ~WindowsInputManager() {
        instance = nullptr;
    }
protected:

    // MAP Button -> Key Val
    unsigned char Buttons[IM__BUTTONS_NUMBER] = {
        QUIT_KEY,
        PAUSE_KEY,
        ENTER_KEY,
        LEFT_KEY,
        RIGHT_KEY,
        UP_KEY,
        DOWN_KEY,
        N_KEY,
        D0_KEY,
        D1_KEY,
        D2_KEY,
        D3_KEY,
        D4_KEY,
        D5_KEY,
        D6_KEY,
        D7_KEY,
        D8_KEY,
        D9_KEY,
        NOTHING_KEY
    };
};