#pragma once

#include <vector>


#ifdef WINDOWS
#include <Windows.h>
#endif


#ifdef WINDOWS
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

#define IM__KEYS_NUMBER 256

#endif



enum Button {
    QUIT_BTN,
    PAUSE_BTN,
    ENTER_BTN,
    LEFT_BTN,
    RIGHT_BTN,
    UP_BTN,
    DOWN_BTN,
    NOTHING
};

#define IM__BUTTONS_NUMBER 8


class InputManager {
public:
    // If more than one button is pressed in a frame, the class will decide wich
    // key ha priority ( QUIT -> PAUSE -> ENTER -> LEFT -> RIGHT -> UP -> DOWN )
    Button getPressedButton();

    // List of all buttons pressed during the frame
    std::vector<Button> getButtonsPressed();
    static InputManager * getInstance();
    static int Buttons[IM__BUTTONS_NUMBER];
private:
    InputManager();
    static InputManager * instance;
    //unsigned char KeyboardStatus[IM__KEYS_NUMBER]; //256 bytes nell'heap, non mi sembra un'esagerazione ...
    
};