//My includes
#include "Types.h"
#include "SystemFactory.h"
#include "GameManager.h"
#include "InputManager.h"
#include "FakePhysicsManager.h"
#include "OSHelper.h"

#include "AssetsManager.h"

//ThirdParty includes
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//Includes reserved for windows builds
#ifdef WINDOWS 
#include "WindowsSystemFactory.h"
#include <Windows.h>
#endif // WINDOWS

//C++ Includes
#include <iostream>


//TMP 4 tests
#include "Block.h"
#include "Character.h"


int ID::_ID = 0;

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
    FakePhysicsManager * physicsManager = FakePhysicsManager::getInstance();
    AssetsManager * assetsManager = AssetsManager::getInstance();


    //Setup & Inizialization of all components
    assetsManager->preloadAllAssets();
    gameManager->initialization(systemFactory);
    

    //Events subscriptions
    gameManager->OnButtonPressed[UP_BTN].subscribe(std::bind(&GraphicsManager::CameraGoUp, graphicsManager)); //TODO REMOVE

    bool not3DError = true;

    


    physicsManager->initialization(gameManager->getPlayerInCurrentLevel(), gameManager->getBallInCurrentLevel());


    graphicsManager->setScene(gameManager->getCurrentLevelScene());
    graphicsManager->setup(gameManager->getGameConfiguration());
    not3DError = graphicsManager->initialization();
    if(not3DError)
        not3DError = graphicsManager->run();

    //Main Cycle
    while (!GameManager::quit && not3DError) {

        //step -1 : start clock to control FPS
        gameManager->startCheckFPS();

        //step 0 : Input
        Button action = inputManager->getButtonPressed();

        //step 1 : fireEvents
        gameManager->processEvent(action);

        //step 2 : physics
        physicsManager->doPhysicUpdate();

        //step 3 : Render:
        not3DError = graphicsManager->render();

        // final step : check FPS and wait if necessary
        gameManager->checkFPS();

    }

    graphicsManager->shutdown();


    SAFE_DELETE(osHelper);
    SAFE_DELETE(inputManager);
    SAFE_DELETE(gameManager);
    SAFE_DELETE(graphicsManager);
    SAFE_DELETE(systemFactory);
    SAFE_DELETE(physicsManager);
    SAFE_DELETE(assetsManager);

    return !not3DError;
}