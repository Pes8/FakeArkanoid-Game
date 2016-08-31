#pragma once

#include "Types.h"
#include "InputManager.h"
#include "OSHelper.h"
#include "SystemFactory.h"
#include "Character.h"
#include "Ball.h"
#include "UIObject.h"
#include "UIText.h"
#include <chrono>

struct Level {
    int levelNumber;
    std::vector<GameObject *> m_oObjectsList;
    Character * m_oPlayer;
    Ball * m_oBall;
    ~Level();
};

class GameManager{
public:
    static bool quit;
    static GameManager * getInstance();
    void initialization(SystemFactory * _systemFactory);
    void processEvent(Button pressedButton);
    Event<void> OnButtonPressed[IM__BUTTONS_NUMBER];
    GameConfig * getGameConfiguration() const;
    void startCheckFPS();
    void checkFPS();
    const std::vector<GameObject *> * getCurrentLevelScene() const;
    const std::vector<UIText*> * getCurrentUI() const;
    const GameObject * getPlayerInCurrentLevel() const;
    const GameObject * getBallInCurrentLevel() const;
    ~GameManager();
    
private:
    
    static GameManager * instance;
    OSHelper * m_oSystemHelper;
    GameConfig * config;
    int m_iCurrentLevelNumber;
    Level * m_oCurrentLevel;
    std::vector<UIText*> m_oCurrentUI;
    std::chrono::high_resolution_clock::time_point m_oCurrentFrameStartTime;

    void resetGame();
    void prepareLevel();
    bool readLevel(int _level);
    void loadRandomLevel();

    int P_Subscription_LEFT_BTN;
    int P_Subscription_RIGHT_BTN;
    int P_Subscription_BALL_OUTSIDE;

    int Subscription_QUIT_BTN;
    int Subcription_BALL_OUTSIDE;
    int Subscription_PLAYER_DEATH;
    int Subscription_D09_BTN[10];
    void OnBallExit();
    void OnBlockDestroyed(int id);
    void OnPlayerDeath();
    void OnLoadLevel(int _level = 0);
    GameManager();
    void OnQuitBtn();

    static char buffer[33];

    UIText * m_oUILivesText;
#if UI_SHOW_FPS
    UIText * m_oUIFPSText = nullptr;
#endif // DEBUG

};