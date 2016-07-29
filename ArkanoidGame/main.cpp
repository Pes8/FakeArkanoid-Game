//My includes
#include "Types.h"
#include "GameManager.h"
#include "InputManager.h"
#include "SystemFactory.h"
#include "OSHelper.h"

#ifdef WINDOWS //Includes reserved for windows builds
#include "WindowsSystemFactory.h"
#include <Windows.h>
#endif // WINDOWS

//C++ Includes
#include <iostream>


#ifdef WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif

#ifdef WINDOWS
    SystemFactory * systemFactory = new WindowsSystemFactory();
#endif // WINDOWS

    GameManager * gameManager = GameManager::getInstance();
    InputManager * inputManager = systemFactory->getInputManager();
    GraphicsManager * graphicsManager = systemFactory->getGraphicsManager();
    OSHelper * osHelper = systemFactory->getOSHelper();

    //Setup & Inizialization of all components
    gameManager->initialization(systemFactory);
    
    graphicsManager->setup(gameManager->getGameConfiguration());
    graphicsManager->initialization();
    graphicsManager->run();


    //Main Cycle
    while (!GameManager::quit) {

        //step -1 : start clock to control FPS
        gameManager->startCheckFPS();

        //step 0 : Input
        Button action = inputManager->getButtonPressed();

        //step 1 : fireEvents
        gameManager->processEvent(action);

        //step 3 : Render:
        graphicsManager->render();

        // final step : check FPS and wait if necessary
        gameManager->checkFPS();

    }

    graphicsManager->shutdown();


    return 0;
}