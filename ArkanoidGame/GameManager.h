#pragma once

#include "InputManager.h"

class GameManager{
public:
    static bool quit;
    static GameManager * getInstance();
    void inizialization();
    void processEvent(Button pressedButton);
private:
    GameManager();
    static GameManager * instance;
};