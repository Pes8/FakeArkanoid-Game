#include "InputManager.h"
#include <cstring>

InputManager * InputManager::instance = nullptr;

// MAP Button -> Key Val
int InputManager::Buttons[IM__BUTTONS_NUMBER] = {
    QUIT_KEY,
    PAUSE_KEY,
    ENTER_KEY,
    LEFT_KEY,
    RIGHT_KEY,
    UP_KEY,
    DOWN_KEY,
    NOTHING_KEY
};

Button InputManager::getPressedButton(){
    for (int i = 0; i < IM__BUTTONS_NUMBER; ++i) {
        if (GetAsyncKeyState(Buttons[i]) & 0x8000) {
            return static_cast<Button>(i);
        }
    }
}

std::vector<Button> InputManager::getButtonsPressed(){
    //std::memset(KeyboardStatus, 0, sizeof(KeyboardStatus));
    //GetKeyboardState(KeyboardStatus);
    std::vector<Button> pressedButtons;
    
    pressedButtons.push_back(NOTHING);

    for (int i = 0; i < IM__BUTTONS_NUMBER; ++i) {
        if (GetAsyncKeyState(Buttons[i]) & 0x8000) {
            pressedButtons.push_back(static_cast<Button>(i));
        }
    }

    return pressedButtons;
}

InputManager * InputManager::getInstance(){
    if (InputManager::instance == nullptr)
        InputManager::instance = new InputManager();
    return InputManager::instance;
}

InputManager::InputManager(){
    
    

}
