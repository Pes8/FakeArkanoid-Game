#include "Types.h"
#include "GameManager.h"
#include "InputManager.h"
#include <iostream>
//#include <bitset>


void OnQuitPressed() {
    std::cout << "Premuto ESC!" << std::endl;
}


void OnEnterPressed() {
    std::cout << "Premuto ENTER!" << std::endl;
}

int main() {

    GameManager * gameManager = GameManager::getInstance();
    InputManager * inputManager = InputManager::getInstance();

    //Inizialization of all components
    gameManager->inizialization();

    gameManager->OnButtonPressed[QUIT_BTN].subscribe(OnQuitPressed);
    gameManager->OnButtonPressed[ENTER_BTN].subscribe(OnEnterPressed);

    //Main Cycle
    while (!GameManager::quit) {
        //step 0 : getInputKey

        ButtonsStatus btnS = inputManager->getButtonsPressed();
        /*std::bitset<8> x(btnS);
        std::cout << x << std::endl;
        */
        Button action = inputManager->getButtonPressed();

        std::cout << action << std::endl;

        gameManager->processEvent(action);

        

        //step 1 : fireEvents

        //step 3 : execute all engine (graphics, physics)


    }

    return 0;
}