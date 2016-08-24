#pragma once

#include "Types.h"
#include "InputManager.h"
#include "OSHelper.h"
#include "SystemFactory.h"
#include "Character.h"
#include "Ball.h"
#include <chrono>

struct Level {
    int levelNumber;
    std::vector<GameObject *> m_oObjectsList;
    Character * m_oPlayer;
    Ball * m_oBall;
};

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
    const std::vector<GameObject *> * getCurrentLevelScene() const;
    const GameObject * getPlayerInCurrentLevel() const;
    const GameObject * getBallInCurrentLevel() const;
private:
    
    static GameManager * instance;
    OSHelper * m_oSystemHelper;
    GameConfig * config;
    int m_iCurrentLevelNumber;
    Level * m_oCurrentLevel;
    std::chrono::high_resolution_clock::time_point m_oCurrentFrameStartTime;

    bool readLevel(int _level);

    GameManager();
    void OnQuitBtn();
};