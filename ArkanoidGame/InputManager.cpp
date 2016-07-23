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

Button InputManager::getButtonPressed(){
    
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
        bool pressed = (btnS & (1 << i) ) > 0; //added > 0 beacuse useless compiler warning 
        if(pressed)
            return static_cast<Button>(i);
    }


    return NOTHING;
}

ButtonsStatus InputManager::getButtonsPressed(){
    
    ButtonsStatus btnS = 0x0;


#ifdef WINDOWS
    for (int i = 0; i < IM__BUTTONS_NUMBER - 1; ++i) {
        btnS |= ((GetAsyncKeyState(Buttons[i]) && 0x8000) << i);
    }
#endif // WINDOWS
    

    return btnS;
}

InputManager * InputManager::getInstance(){
    if (InputManager::instance == nullptr)
        InputManager::instance = new InputManager();
    return InputManager::instance;
}

InputManager::InputManager() {

}