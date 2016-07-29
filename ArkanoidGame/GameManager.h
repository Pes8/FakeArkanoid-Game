#pragma once

#include "Types.h"
#include "InputManager.h"
#include "OSHelper.h"
#include "SystemFactory.h"
#include <chrono>


class GameManager{
public:
    static bool quit;
    static GameManager * getInstance();
    void initialization(SystemFactory * _systemFactory);
    void processEvent(Button pressedButton);
    Event OnButtonPressed[IM__BUTTONS_NUMBER];
    GameConfig * getGameConfiguration() const;
    void startCheckFPS();
    void checkFPS();
private:
    
    static GameManager * instance;
    OSHelper * m_oSystemHelper;
    GameConfig * config;

    std::chrono::high_resolution_clock::time_point m_oCurrentFrameStartTime;

    GameManager();
    void OnQuitBtn();
};