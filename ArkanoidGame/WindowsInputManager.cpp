#include "WindowsInputManager.h"

InputManager * WindowsInputManager::getInstance(){
    if (WindowsInputManager::instance == nullptr)
        WindowsInputManager::instance = new WindowsInputManager();
    return WindowsInputManager::instance;
}

Button WindowsInputManager::getButtonPressed() {

    ButtonsStatus btnS = getButtonsPressed();

    for (unsigned int i = 0; i < IM__BUTTONS_NUMBER - 1; ++i) {
        bool pressed = (btnS & (1 << i)) > 0; //added > 0 beacuse useless compiler warning 
        if (pressed)
            return static_cast<Button>(i);
    }

    return NOTHING;
}

ButtonsStatus WindowsInputManager::getButtonsPressed() {

    ButtonsStatus btnS = 0x0;

    for (unsigned int i = 0; i < IM__BUTTONS_NUMBER - 1; ++i) {
        btnS |= ((GetAsyncKeyState(Buttons[i]) && 0x8000) << i);
    }

    return btnS;
}