#include "GameManager.h"
#include "AssetsManager.h"
#include <fstream>
#include <sstream>

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

    readLevel(0);
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

const std::vector<GameObject*> * GameManager::getCurrentLevelScene() const {
    return &(m_oCurrentLevel->m_oObjectsList);
}

const GameObject * GameManager::getPlayerInCurrentLevel() const {
    return m_oCurrentLevel->m_oPlayer;
}

const GameObject * GameManager::getBallInCurrentLevel() const {
    return m_oCurrentLevel->m_oBall;
}

bool GameManager::readLevel(int _level) {
    
    AssetsManager * assetsManager = AssetsManager::getInstance();

    //Walls, one player and one ball exists always
    Character * player = assetsManager->createPlayer();
    Ball * ball = assetsManager->createBall();

    Wall * wsx = assetsManager->createVerticalWall();
    wsx->m_vPosition = { -10.25,0.0f,0.0f };

    Wall * wdx = assetsManager->createVerticalWall();
    wdx->m_vPosition = { 10.25,0.0f,0.0f };

    Wall * wup = assetsManager->createHorizontalWall();
    wup->m_vPosition = { 0.0f, 10.0f, 0.0f };
    
    m_oCurrentLevel->m_oPlayer = player;
    m_oCurrentLevel->m_oBall = ball;

    m_oCurrentLevel->m_oObjectsList.push_back(ball);
    m_oCurrentLevel->m_oObjectsList.push_back(player);
    m_oCurrentLevel->m_oObjectsList.push_back(wsx);
    m_oCurrentLevel->m_oObjectsList.push_back(wdx);
    m_oCurrentLevel->m_oObjectsList.push_back(wup);


    std::ifstream infile(LEVEL_PATH "L"+std::to_string(_level));
    if (!infile.is_open()) return false;

    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        std::string code;
        iss >> code;
        
        if (code == "BA") {
            char c;
            iss >> c;
            if (c == 'V') {
                float vel;
                iss >> vel;
                ball->m_vVelocity.y = -vel;
            }
            iss >> c;
            if (c == 'P') {
                Vector3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                ball->m_vPosition = pos;
            }
            
        } else if (code == "BL") {
            int type;
            iss >> type;
            char c;
            iss >> c;
            if (c == 'P') {
                Vector3 pos = { -9.5f, 9.5f, 0.0f };
                int i, j;
                iss >> i >> j;
                pos.x += i * GRID_BLOCK_SIZE_X;
                pos.y -= j * GRID_BLOCK_SIZE_Y;

                Block * b = assetsManager->createBlock(type);
                b->m_vPosition = pos;
                m_oCurrentLevel->m_oObjectsList.push_back(b);
            }
        }

    }

    OnButtonPressed[LEFT_BTN].subscribe(std::bind(&Character::CharGoLeft, player)); //TODO REMOVE
    OnButtonPressed[RIGHT_BTN].subscribe(std::bind(&Character::CharGoRight, player)); //TODO REMOVE

    return true;
}

GameManager::GameManager(){
    config = new GameConfig();
    m_oCurrentLevel = new Level();
}

void GameManager::OnQuitBtn(){
    GameManager::quit = true;
}
