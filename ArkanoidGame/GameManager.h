#pragma once

#include "Types.h"
#include "InputManager.h"

class GameManager{
public:
    static bool quit;
    static GameManager * getInstance();
    void inizialization();
    void processEvent(Button pressedButton);
    Event OnButtonPressed[IM__BUTTONS_NUMBER];
private:
    static GameManager * instance;
    GameManager();
    void OnQuitBtn();
};