typedef void(*FP)();

#include "GameManager.h"
#include "InputManager.h"
#include <iostream>

class InputManager;
class EventManager;
class RenderingEngine;
class GameLogic;
class Physics;






int main() {

    GameManager * gameManager = GameManager::getInstance();
    InputManager * inputManager = InputManager::getInstance();

    //Inizialization of all components
    gameManager->inizialization();


    //Main Cycle
    while (!GameManager::quit) {
        //step 0 : getInputKey

        Button button = inputManager->getPressedButton();

        gameManager->processEvent(button);

        std::cout << "k" << std::endl;

        //step 1 : fireEvents

        //step 3 : execute all engine (graphics, physics)


    }

    return 0;
}