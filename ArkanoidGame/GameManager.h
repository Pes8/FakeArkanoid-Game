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
    
    static GameManager * getInstance();
    void initialization(SystemFactory * _systemFactory);
    void processEvent(Button pressedButton);
    GameConfig * getGameConfiguration() const;
    void startCheckFPS();
    void checkFPS();
    const std::vector<GameObject *> * getCurrentLevelScene() const;
    const std::vector<UIText*> * getCurrentUI() const;
    const GameObject * getPlayerInCurrentLevel() const;
    const GameObject * getBallInCurrentLevel() const;
    GameState getGameState() const;
    ~GameManager();
  
    Event<void> OnButtonPressed[IM__BUTTONS_NUMBER];
    
private:

    GameManager();
    void resetGame();
    void prepareLevel();
    bool readLevel(int _level);
    void loadRandomLevel();
    void OnBallExit();
    void OnBlockDestroyed(int id);
    void OnPlayerDeath();
    void OnLoadLevel(int _level = 0);
    void OnQuitBtn();
    bool readConfigFromFile();


    std::vector<UIText*> m_oCurrentUI;
    std::chrono::high_resolution_clock::time_point m_oCurrentFrameStartTime;

    static GameManager * instance;
    OSHelper * m_oSystemHelper;
    GameConfig * config;
    int m_iCurrentLevelNumber;
    Level * m_oCurrentLevel;

    int P_Subscription_LEFT_BTN;
    int P_Subscription_RIGHT_BTN;
    int P_Subscription_BALL_OUTSIDE;
    int Subscription_QUIT_BTN;
    int Subcription_BALL_OUTSIDE;
    int Subscription_PLAYER_DEATH;
    int Subscription_D09_BTN[10];
    
    GameState m_eState;

    static char buffer[33];

    UIText * m_oUILivesText;
#if UI_SHOW_FPS
    UIText * m_oUIFPSText = nullptr;
#endif // DEBUG

};