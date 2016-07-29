//My includes
#include "Types.h"
#include "GameManager.h"
#include "InputManager.h"
#include "SystemFactory.h"

#ifdef WINDOWS //Includes reserved for windows builds
#include "WindowsSystemFactory.h"
#include <Windows.h>
#endif // WINDOWS

//C++ Includes
#include <iostream>
#include <chrono>



#define FPS 120 // Frame Per Seconds
#define uSPF (1000000 / FPS) // Micro-Seconds Per Frame


/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {*/
int main(){

#ifdef WINDOWS
    SystemFactory * systemFactory = new WindowsSystemFactory();
#endif // WINDOWS

    GameManager * gameManager = GameManager::getInstance();
    InputManager * inputManager = systemFactory->getInputManager();

    //Inizialization of all components
    gameManager->inizialization();


    //Main Cycle
    while (!GameManager::quit) {

        //Start time to lock the cycle to max FPS
        auto startTime = std::chrono::high_resolution_clock::now();

        //step 0 : Input
        Button action = inputManager->getButtonPressed();
        gameManager->processEvent(action);

        std::cout << "X" << std::endl;

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