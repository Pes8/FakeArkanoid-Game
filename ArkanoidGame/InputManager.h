#pragma once

#include <vector>
#include "Types.h"

#define IM__BUTTONS_NUMBER 19

class InputManager {
public:
    // If more than one button is pressed in a frame, the class will decide wich
    // key ha priority ( QUIT -> PAUSE -> ENTER -> LEFT -> RIGHT -> UP -> DOWN )
    virtual Button getButtonPressed() = 0;

    // List of all buttons pressed during the frame
    virtual ButtonsStatus getButtonsPressed() = 0;
    virtual ~InputManager() = 0;
    static InputManager * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    static InputManager * instance;
};