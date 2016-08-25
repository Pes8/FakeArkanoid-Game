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
    Subscription_QUIT_BTN = OnButtonPressed[QUIT_BTN].subscribe(std::bind(&GameManager::OnQuitBtn, this));
    m_oSystemHelper = _systemFactory->getOSHelper(); // singleton, return only pointer

    readLevel(1);
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

GameManager::~GameManager() {
    OnButtonPressed[LEFT_BTN].unsubscribe(P_Subscription_LEFT_BTN); //TODO check if is ok
    OnButtonPressed[RIGHT_BTN].unsubscribe(P_Subscription_RIGHT_BTN); //TODO check if is ok
    OnButtonPressed[QUIT_BTN].unsubscribe(Subscription_QUIT_BTN);

    delete m_oCurrentLevel;
}

bool GameManager::readLevel(int _level) {
    
    AssetsManager * assetsManager = AssetsManager::getInstance();

    //Walls, a player and a ball always exists
    Character * player = assetsManager->createPlayer();
    Ball * ball = assetsManager->createBall();

    Wall * wsx = assetsManager->createVerticalWall();
    wsx->m_vPosition = { -CAMERA_ORTO_WIDTH/2, 0.0f,0.0f };

    Wall * wdx = assetsManager->createVerticalWall();
    wdx->m_vPosition = { CAMERA_ORTO_WIDTH/2, 0.0f,0.0f };

    Wall * wup = assetsManager->createHorizontalWall();
    wup->m_vPosition = { 0.0f, CAMERA_ORTO_HEIGHT/2, 0.0f };
    
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
                Vector3 pos = { GRID_BLOCK_START_X, GRID_BLOCK_START_Y, 0.0f };
                int i, j;
                iss >> i >> j;
                pos.x += i * GRID_BLOCK_SIZE_X;
                pos.y -= j * GRID_BLOCK_SIZE_Y;

                Block * b = assetsManager->createBlock(type);
                b->m_vPosition = pos;
                b->m_eBlockDestroyed.subscribe(std::bind(&GameManager::OnBlockDestroyed, this, std::placeholders::_1));
                m_oCurrentLevel->m_oObjectsList.push_back(b);
            }
        }

    }

    Subscription_PLAYER_DEATH = player->m_ePlayerDeath.subscribe(std::bind(&GameManager::OnPlayerDeath, this));
    Subcription_BALL_OUTSIDE = ball->m_eBallOutsideScreen.subscribe(std::bind(&GameManager::OnBallExit, this));

    P_Subscription_LEFT_BTN = OnButtonPressed[LEFT_BTN].subscribe(std::bind(&Character::CharGoLeft, player)); //TODO REMOVE
    P_Subscription_RIGHT_BTN = OnButtonPressed[RIGHT_BTN].subscribe(std::bind(&Character::CharGoRight, player)); //TODO REMOVE
    P_Subscription_BALL_OUTSIDE = ball->m_eBallOutsideScreen.subscribe(std::bind(&Character::LostLife, player)); // TODO check if it's correct ..

    return true;
}

GameManager::GameManager(){
    config = new GameConfig();
    m_oCurrentLevel = new Level();
}

void GameManager::OnQuitBtn(){
    GameManager::quit = true;
}

void GameManager::OnBallExit() {
    //RESET BALL Position & Velocity
    m_oCurrentLevel->m_oBall->m_vPosition = BALL_POSITION;
    m_oCurrentLevel->m_oBall->m_vVelocity = { 0.0f, BALL_VELOCITY, 0.0f };
    int x = 333;
}

void GameManager::OnBlockDestroyed(int id) {
    for (auto it = m_oCurrentLevel->m_oObjectsList.begin(); it != m_oCurrentLevel->m_oObjectsList.end(); ++it){
        if ((*it)->_MyID == id) {
            //delete *it; // Can't delete now, because event need to propagate. Block will suicide
            m_oCurrentLevel->m_oObjectsList.erase(it);

            break;
        }
    }

    if (m_oCurrentLevel->m_oObjectsList.size() <= 5) { // player, ball and 3 walls
        // WIN!!
        int x = 113;
    }
}

void GameManager::OnPlayerDeath() {
    //Game Over!
    int x = 112;
    //m_oCurrentLevel->m_oBall->m_eBallOutsideScreen.unsubscribe(P_Subscription_BALL_OUTSIDE);
    //m_oCurrentLevel->m_oPlayer->m_ePlayerDeath.unsubscribe(Subscription_PLAYER_DEATH);
}

Level::~Level() {
    CLEAR_POINTER_CONTAINER(m_oObjectsList);
    m_oBall = nullptr;
    m_oPlayer = nullptr;
}