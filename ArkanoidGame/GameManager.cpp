#include "GameManager.h"

bool GameManager::quit = false;
GameManager * GameManager::instance = nullptr;

GameManager * GameManager::getInstance(){
    if (GameManager::instance == nullptr)
        GameManager::instance = new GameManager();

    return GameManager::instance;
}

void GameManager::initialization(SystemFactory * _systemFactory){
    OnButtonPressed[QUIT_BTN].subscribe(std::bind(&GameManager::OnQuitBtn, this));
    m_oSystemHelper = _systemFactory->getOSHelper(); // singleton, return only pointer
}

void GameManager::processEvent(Button pressedButton){
    OnButtonPressed[pressedButton].fire();
}

GameConfig * GameManager::getGameConfiguration() const {
    return config;
}

void GameManager::startCheckFPS() {
    m_oCurrentFrameStartTime = std::chrono::high_resolution_clock::now();
}

void GameManager::checkFPS() {
    auto endTime = std::chrono::high_resolution_clock::now();
    unsigned int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_oCurrentFrameStartTime).count();
    if (frameTime < uSPF) {
        m_oSystemHelper->sleep_uS(uSPF - frameTime);
    }
}

GameManager::GameManager(){
    config = new GameConfig();
}

void GameManager::OnQuitBtn(){
    GameManager::quit = true;
}
