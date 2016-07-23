#include "GameManager.h"

bool GameManager::quit = false;
GameManager * GameManager::instance = nullptr;

GameManager * GameManager::getInstance(){
    if (GameManager::instance == nullptr)
        GameManager::instance = new GameManager();

    return GameManager::instance;
}

void GameManager::inizialization(){
    //GameManager::quit = false;
}

void GameManager::processEvent(Button pressedButton){
    
    //Switch è meglio di una serie di if (anche se i compilatori sono molto intelligenti). TODO inserire riferimento a quanto detto.
    /*switch (pressedButton) {
    case QUIT_BTN :
        GameManager::quit = true;
        break;
    case PAUSE_KEY:
        //Fire Event PAUSE
        break;
    case ENTER_KEY:
        break;
    case LEFT_KEY:
        break;
    case RIGHT_KEY:
        break;
    case UP_KEY:
        break;
    case DOWN_KEY:
        break;
    default:
        break;
    }*/
    OnButtonPressed[pressedButton].fire();
}

GameManager::GameManager(){
}
