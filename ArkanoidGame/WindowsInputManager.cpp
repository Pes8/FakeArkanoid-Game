#include "WindowsInputManager.h"
#include <cstring>

InputManager * WindowsInputManager::getInstance(){
    if (WindowsInputManager::instance == nullptr)
        WindowsInputManager::instance = new WindowsInputManager();
    return WindowsInputManager::instance;
}

WindowsInputManager::~WindowsInputManager() {
    instance = nullptr;
}


Button WindowsInputManager::getButtonPressed() {

    ButtonsStatus btnS = getButtonsPressed();

    //TODO: find a solution without if-else
    /*for (int i = 1; i < IM__BUTTONS_NUMBER; ++i) {

    bool f = btnS & i;

    for (int j = i+1; j < IM__BUTTONS_NUMBER; ++j) {
    bool curr = btnS & j;
    bool res = (f | curr) ^ curr;
    btnS |= (res << j);
    }
    }*/

    for (unsigned int i = 0; i < IM__BUTTONS_NUMBER - 1; ++i) {
        bool pressed = (btnS & (1 << i)) > 0; //added > 0 beacuse useless compiler warning 
        if (pressed)
            return static_cast<Button>(i);
    }

    return NOTHING;
}

ButtonsStatus WindowsInputManager::getButtonsPressed() {

    ButtonsStatus btnS = 0x0;

    for (int i = 0; i < IM__BUTTONS_NUMBER - 1; ++i) {
        btnS |= ((GetAsyncKeyState(Buttons[i]) && 0x8000) << i);
    }

    return btnS;
}