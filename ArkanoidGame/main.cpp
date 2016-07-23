#include "Types.h"
#include "GameManager.h"
#include "InputManager.h"
#include <iostream>
#include <chrono>

//Includes reserved for windows builds
#ifdef WINDOWS

#endif

//Includes reserved for linux builds
#ifdef LINUX
#include <unistd.h>
#endif


#define FPS 22
#define uSPF (1000000 / FPS) //micro-seconds for frame


int main() {

    GameManager * gameManager = GameManager::getInstance();
    InputManager * inputManager = InputManager::getInstance();

    //Inizialization of all components
    gameManager->inizialization();


    //Main Cycle
    while (!GameManager::quit) {

        //Start time to lock the cycle to max FPS
        auto startTime = std::chrono::high_resolution_clock::now();

        //step 0 : Input
        Button action = inputManager->getButtonPressed();
        gameManager->processEvent(action);

        

        //step 1 : fireEvents

        //step 3 : execute all engine (graphics, physics)

        //ugly code .. I hate std::chrono ...
        auto endTime = std::chrono::high_resolution_clock::now();
        unsigned int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        if (frameTime < uSPF) {
#ifdef WINDOWS
            Sleep((uSPF - frameTime)/1000);
#elif LINUX
            usleep(uSPF - frameTime);
#endif
        }
    }

    return 0;
}