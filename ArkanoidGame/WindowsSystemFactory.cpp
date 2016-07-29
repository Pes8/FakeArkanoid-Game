#include "WindowsSystemFactory.h"
#include "WindowsInputManager.h"
#include "WindowsGraphicsManager.h"
#include "WindowsHelper.h"

InputManager * WindowsSystemFactory::getInputManager(){
    return WindowsInputManager::getInstance();
}

GraphicsManager * WindowsSystemFactory::getGraphicsManager(){
    return WindowsGraphicsManager::getInstance();
}

OSHelper * WindowsSystemFactory::getOSHelper() {
    return WindowsHelper::getInstance();
}
