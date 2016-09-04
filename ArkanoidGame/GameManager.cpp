#include "GameManager.h"
#include "AssetsManager.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

char GameManager::buffer[33] = {0};


GameManager * GameManager::instance = nullptr;

GameManager * GameManager::getInstance(){
    if (GameManager::instance == nullptr)
        GameManager::instance = new GameManager();

    return GameManager::instance;
}

void GameManager::initialization(SystemFactory * _systemFactory){
    Subscription_QUIT_BTN = OnButtonPressed[QUIT_BTN].subscribe(std::bind(&GameManager::OnQuitBtn, this));
    Subscription_QUIT_BTN = OnButtonPressed[N_BTN].subscribe(std::bind(&GameManager::loadRandomLevel, this));
    Subscription_D09_BTN[0] = OnButtonPressed[D0_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 0));
    Subscription_D09_BTN[1] = OnButtonPressed[D1_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 1));
    Subscription_D09_BTN[2] = OnButtonPressed[D2_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 2));
    Subscription_D09_BTN[3] = OnButtonPressed[D3_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 3));
    Subscription_D09_BTN[4] = OnButtonPressed[D4_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 4));
    Subscription_D09_BTN[5] = OnButtonPressed[D5_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 5));
    Subscription_D09_BTN[6] = OnButtonPressed[D6_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 6));
    Subscription_D09_BTN[7] = OnButtonPressed[D7_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 7));
    Subscription_D09_BTN[8] = OnButtonPressed[D8_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 8));
    Subscription_D09_BTN[9] = OnButtonPressed[D9_BTN].subscribe(std::bind(&GameManager::OnLoadLevel, this, 9));
    
    m_oSystemHelper = _systemFactory->getOSHelper(); // singleton, return only pointer
    
    prepareLevel();
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

#if UI_SHOW_FPS
    //static char buffer[33];
    unsigned int _realFPS = 1000000 / frameTime;
#endif // DEBUG

    if (LOCK_FPS &&  frameTime < uSPF) {
        m_oSystemHelper->sleep_uS(uSPF - frameTime);
#if UI_SHOW_FPS
        _realFPS = FPS;
#endif // DEBUG
    }

#if UI_SHOW_FPS
    m_oUIFPSText->setText(_itoa(_realFPS, buffer, 10));
#endif // DEBUG
}

const std::vector<GameObject*> * GameManager::getCurrentLevelScene() const {
    return &(m_oCurrentLevel->m_oObjectsList);
}

const std::vector<UIText*>* GameManager::getCurrentUI() const {
    return &m_oCurrentUI;
}

const GameObject * GameManager::getPlayerInCurrentLevel() const {
    return m_oCurrentLevel->m_oPlayer;
}

const GameObject * GameManager::getBallInCurrentLevel() const {
    return m_oCurrentLevel->m_oBall;
}

GameState GameManager::getGameState() const {
    return m_eState;
}

GameManager::~GameManager() {
    OnButtonPressed[LEFT_BTN].unsubscribe(P_Subscription_LEFT_BTN); //TODO check if is ok
    OnButtonPressed[RIGHT_BTN].unsubscribe(P_Subscription_RIGHT_BTN); //TODO check if is ok
    OnButtonPressed[QUIT_BTN].unsubscribe(Subscription_QUIT_BTN);
    CLEAR_POINTER_CONTAINER(m_oCurrentUI);
    delete m_oCurrentLevel;
}

GameManager::GameManager() {
    config = new GameConfig();
    readConfigFromFile();

    m_oCurrentLevel = new Level();

#if UI_SHOW_FPS // UI_SHOW_FPS
    m_oUIFPSText = new UIText(L"30", 2, { 0.0f, 0.0f }, TextType::SMALL);
    m_oCurrentUI.push_back(m_oUIFPSText);
#endif // UI_SHOW_FPS
    m_oUILivesText = new UIText(_itoa(PLAYER_LIVES, buffer, 10), 1, { 100.0f, 570.0f });

    m_oCurrentUI.push_back(new UIText(L"Press 'N' to randomize blocks", 30, { 50.0f, 500.0f }, TextType::SMALL));
    m_oCurrentUI.push_back(new UIText(L"Press '0' to '9' to load a level", 33, { 50.0f, 515.0f }, TextType::SMALL));
    m_oCurrentUI.push_back(new UIText(L"Press 'ESC' to Exit", 20, { 50.0f, 530.0f }, TextType::SMALL));
    m_oCurrentUI.push_back(new UIText(L"Lives: ", 8, { 50.0f, 570.0f }));
    m_oCurrentUI.push_back(m_oUILivesText);

    m_eState = GameState::PLAYING;
}

void GameManager::resetGame() {
    //Original Player Position + restore lives
    m_oCurrentLevel->m_oPlayer->m_vPosition = PLAYER_START_POS;
    m_oCurrentLevel->m_oPlayer->m_vVelocity = {0.0f, 0.0f, 0.0f};
    m_oCurrentLevel->m_oPlayer->m_iLives = config->startLife;
    m_oUILivesText->setText(_itoa(m_oCurrentLevel->m_oPlayer->m_iLives, buffer, 10));

    //RESET BALL Position & Velocity
    m_oCurrentLevel->m_oBall->m_vPosition = BALL_POSITION;
    m_oCurrentLevel->m_oBall->m_vVelocity = { 0.0f, BALL_VELOCITY, 0.0f };

    // erase only the blocks
    CLEAR_POINTER_CONTAINER_FROM_TO(m_oCurrentLevel->m_oObjectsList, m_oCurrentLevel->m_oObjectsList.begin() + 5, m_oCurrentLevel->m_oObjectsList.end());

    //Clean UI
#if UI_SHOW_FPS
    m_oCurrentUI.erase(m_oCurrentUI.begin() + 6, m_oCurrentUI.end());
#else
    m_oCurrentUI.erase(m_oCurrentUI.begin() + 5, m_oCurrentUI.end());
#endif // SHOW_FPS

    
}

void GameManager::prepareLevel() {
    AssetsManager * assetsManager = AssetsManager::getInstance();

    //Walls, a player and a ball always exists
    Character * player = assetsManager->createPlayer();
    player->m_iLives = config->startLife;

    Ball * ball = assetsManager->createBall();

    Wall * wsx = assetsManager->createVerticalWall();
    wsx->m_vPosition = { -CAMERA_ORTO_WIDTH / 2, 0.0f,0.0f };

    Wall * wdx = assetsManager->createVerticalWall();
    wdx->m_vPosition = { CAMERA_ORTO_WIDTH / 2, 0.0f,0.0f };

    Wall * wup = assetsManager->createHorizontalWall();
    wup->m_vPosition = { 0.0f, CAMERA_ORTO_HEIGHT / 2, 0.0f };

    m_oCurrentLevel->m_oPlayer = player;
    m_oCurrentLevel->m_oBall = ball;

    m_oCurrentLevel->m_oObjectsList.push_back(ball);
    m_oCurrentLevel->m_oObjectsList.push_back(player);
    m_oCurrentLevel->m_oObjectsList.push_back(wsx);
    m_oCurrentLevel->m_oObjectsList.push_back(wdx);
    m_oCurrentLevel->m_oObjectsList.push_back(wup);

    Subcription_BALL_OUTSIDE = m_oCurrentLevel->m_oBall->m_eBallOutsideScreen.subscribe(std::bind(&GameManager::OnBallExit, this));
    P_Subscription_BALL_OUTSIDE = m_oCurrentLevel->m_oBall->m_eBallOutsideScreen.subscribe(std::bind(&Character::LostLife, m_oCurrentLevel->m_oPlayer)); // TODO check if it's correct ..
    Subscription_PLAYER_DEATH = m_oCurrentLevel->m_oPlayer->m_ePlayerDeath.subscribe(std::bind(&GameManager::OnPlayerDeath, this));
    P_Subscription_LEFT_BTN = OnButtonPressed[LEFT_BTN].subscribe(std::bind(&Character::CharGoLeft, m_oCurrentLevel->m_oPlayer)); //TODO REMOVE
    P_Subscription_RIGHT_BTN = OnButtonPressed[RIGHT_BTN].subscribe(std::bind(&Character::CharGoRight, m_oCurrentLevel->m_oPlayer)); //TODO REMOVE
}

bool GameManager::readLevel(int _level) {
    
    AssetsManager * assetsManager = AssetsManager::getInstance();

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
                m_oCurrentLevel->m_oBall->m_vVelocity.y = -vel;
            }
            iss >> c;
            if (c == 'P') {
                Vector3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                m_oCurrentLevel->m_oBall->m_vPosition = pos;
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

    return true;
}

void GameManager::loadRandomLevel() {

    if (m_oCurrentLevel != nullptr) {
        resetGame();
    } else {
        m_oCurrentLevel = new Level();
        prepareLevel();
    }

    AssetsManager * assetsManager = AssetsManager::getInstance();
    
    srand(time(NULL));
    unsigned short limit = rand() % 10;
    for (unsigned short i = 0; i < 20; ++i) {
        for (unsigned short j = 0; j < 20; ++j) {
            bool putIt = (rand() % 10) >= limit;
            if (putIt) {
                Vector3 pos = { GRID_BLOCK_START_X, GRID_BLOCK_START_Y, 0.0f };
                pos.x += i * GRID_BLOCK_SIZE_X;
                pos.y -= j * GRID_BLOCK_SIZE_Y;
                unsigned short type = rand() % BLOCK_TYPES;
                Block * b = assetsManager->createBlock(type);
                b->m_vPosition = pos;
                b->m_eBlockDestroyed.subscribe(std::bind(&GameManager::OnBlockDestroyed, this, std::placeholders::_1));
                m_oCurrentLevel->m_oObjectsList.push_back(b);
            }
        }
    }

}

void GameManager::OnQuitBtn() {
    m_eState = GameState::EXIT;
}

bool GameManager::readConfigFromFile() {
    std::ifstream infile(LOCAL_PATH CONFIG_FILE);
    if (!infile.is_open()) return false;

    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        std::string code;
        iss >> code;

        if (code == "SW") {
            iss >> config->screenWidth;
        } else if (code == "SH") {
            iss >> config->screenHeight;
        } else if (code == "SL") {
            iss >> config->startLife;
        } else if (code == "FS") {
            iss >> config->fullscreen;
        } else if (code == "VS") {
            iss >> config->vsyncEnabled;
        }

    }

    return true;
}

void GameManager::OnBallExit() {
    //RESET BALL Position & Velocity and update UI
    m_oCurrentLevel->m_oBall->m_vPosition = BALL_POSITION;
    m_oCurrentLevel->m_oBall->m_vVelocity = { 0.0f, BALL_VELOCITY, 0.0f };
    
    m_oUILivesText->setText(_itoa(m_oCurrentLevel->m_oPlayer->m_iLives-1, buffer, 10));
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
        //WIN!!
                                                       
        //RESET BALL Position & Velocity and update UI
        m_oCurrentLevel->m_oBall->m_vPosition = BALL_POSITION;
        m_oCurrentLevel->m_oBall->m_vVelocity = { 0.0f, 0.0f, 0.0f };

        m_oCurrentUI.push_back(new UIText(L"YOU WIN!", 9, { 500.0f, 300.0f }, TextType::LARGE));
    }
}

void GameManager::OnPlayerDeath() {
    //Game Over!
    m_oCurrentLevel->m_oBall->m_vPosition = BALL_POSITION;
    m_oCurrentLevel->m_oBall->m_vVelocity = { 0.0f, 0.0f, 0.0f };
    
    m_oCurrentUI.push_back(new UIText(L"GAME OVER!", 11, { 500.0f, 300.0f }, TextType::LARGE));
}

void GameManager::OnLoadLevel(int _level) {
    if (m_oCurrentLevel != nullptr) {
        resetGame();
    } else {
        m_oCurrentLevel = new Level();
        prepareLevel();
    }


    readLevel(_level);
}

Level::~Level() {
    CLEAR_POINTER_CONTAINER(m_oObjectsList);
    m_oBall = nullptr;
    m_oPlayer = nullptr;
}