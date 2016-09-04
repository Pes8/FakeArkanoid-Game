#pragma once

#include <vector>
#include "Types.h"

#define IM__BUTTONS_NUMBER 19

class InputManager {
public:
    // If more than one button is pressed in a frame, the class will decide wich
    // key has priority ( QUIT -> PAUSE -> ENTER -> LEFT -> RIGHT -> UP -> DOWN )
    virtual Button getButtonPressed() = 0;

    // List of all buttons pressed during the frame
    virtual ButtonsStatus getButtonsPressed() = 0;
    virtual ~InputManager() = 0;
    static InputManager * getInstance() = delete; //Static Method cannot be Virtual. So Child Class MUST implement it. But deleted here.
protected:
    InputManager(){};
    static InputManager * instance;
};