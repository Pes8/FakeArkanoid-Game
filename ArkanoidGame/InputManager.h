#pragma once

#include <vector>
#include "Types.h"


#define IM__BUTTONS_NUMBER 8


class InputManager {
public:
    // If more than one button is pressed in a frame, the class will decide wich
    // key ha priority ( QUIT -> PAUSE -> ENTER -> LEFT -> RIGHT -> UP -> DOWN )
    Button getButtonPressed();

    // List of all buttons pressed during the frame
    ButtonsStatus getButtonsPressed();
    static InputManager * getInstance();
    static int Buttons[IM__BUTTONS_NUMBER];
protected:
    static InputManager * instance;
    InputManager();
};