//My includes
#include "Types.h"
#include "SystemFactory.h"
#include "GameManager.h"
#include "InputManager.h"
#include "FakePhysicsManager.h"
#include "OSHelper.h"
#include "Scene.h"
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
    Scene * scene = Scene::getInstance();

    //Setup & Inizialization of all components
    gameManager->initialization(systemFactory);
    assetsManager->preloadAllAssets();

    //Events subscriptions
    gameManager->OnButtonPressed[UP_BTN].subscribe(std::bind(&GraphicsManager::CameraGoUp, graphicsManager)); //TODO REMOVE

    bool not3DError = true;




    for (int i = 0; i < 20; ++i) {

        Block * b = assetsManager->createBlock(i % 14);
        b->m_vPosition = { -9.5f + i, 9.5f, 0.0f };

        scene->addObject(b);

    }

    Block * wsx = assetsManager->createVerticalWall();
    Block * wdx = assetsManager->createVerticalWall();
    Block * wup = assetsManager->createHorizontalWall();

    wsx->m_vPosition = {-10.25,0.0f,0.0f};
    wdx->m_vPosition = { 10.25,0.0f,0.0f };
    wup->m_vPosition = {0.0f, 10.0f, 0.0f};

    scene->addObject(wsx);
    scene->addObject(wdx);
    scene->addObject(wup);


    Character * player = assetsManager->createPlayer();
    Block * ball = assetsManager->createBall();

    scene->addObject(ball);

    gameManager->OnButtonPressed[LEFT_BTN].subscribe(std::bind(&Character::CharGoLeft, player)); //TODO REMOVE
    gameManager->OnButtonPressed[RIGHT_BTN].subscribe(std::bind(&Character::CharGoRight, player)); //TODO REMOVE

    scene->addObject(player);


    physicsManager->initialization(scene, player, ball);


    graphicsManager->setScene(scene);
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
    SAFE_DELETE(scene);
    SAFE_DELETE(inputManager);
    SAFE_DELETE(gameManager);
    SAFE_DELETE(graphicsManager);
    SAFE_DELETE(systemFactory);
    SAFE_DELETE(physicsManager);
    SAFE_DELETE(assetsManager);

    return !not3DError;
}