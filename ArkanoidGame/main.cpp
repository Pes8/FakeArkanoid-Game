//My includes
#include "Types.h"
#include "GameManager.h"
#include "InputManager.h"
#include "SystemFactory.h"
#include "OSHelper.h"
#include "Scene.h"

//Includes reserved for windows builds
#ifdef WINDOWS 
#include "WindowsSystemFactory.h"
#include <Windows.h>
#endif // WINDOWS

//C++ Includes
#include <iostream>


//TMP 4 tests
#include "Block.h"


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

    Scene * scene = Scene::getInstance();

    //Setup & Inizialization of all components
    gameManager->initialization(systemFactory);
    
    //Events subscriptions
    gameManager->OnButtonPressed[UP_BTN].subscribe(std::bind(&GraphicsManager::CameraGoUp, graphicsManager)); //TODO REMOVE

    bool not3DError = true;

    Block b1;
    b1.loadMesh();
    b1.setPosition({ -1.0f, -1.0f, 0.0f });
    scene->addObject(&b1);

    Block b2;
    b2.loadMesh();
    b2.setPosition({ 2.0f, 3.0f, 0.0f });
    scene->addObject(&b2);

    graphicsManager->setScene(scene);
    graphicsManager->setup(gameManager->getGameConfiguration());
    not3DError = graphicsManager->initialization();
    not3DError = graphicsManager->run();

    //Main Cycle
    while (!GameManager::quit && not3DError) {

        //step -1 : start clock to control FPS
        gameManager->startCheckFPS();

        //step 0 : Input
        Button action = inputManager->getButtonPressed();

        //step 1 : fireEvents
        gameManager->processEvent(action);

        //step 3 : Render:
        not3DError = graphicsManager->render();

        // final step : check FPS and wait if necessary
        gameManager->checkFPS();

    }

    graphicsManager->shutdown();


    SAFE_DELETE(osHelper);
    SAFE_DELETE(scene);
    SAFE_DELETE(inputManager);
    SAFE_DELETE(gameManager);
    SAFE_DELETE(graphicsManager);
    SAFE_DELETE(systemFactory);

    return !not3DError;
}