#include "WindowsSystemFactory.h"
#include "WindowsInputManager.h"
#include "WindowsGraphicsManager.h"

InputManager * WindowsSystemFactory::getInputManager(){
    return WindowsInputManager::getInstance();
}

GraphicsManager * WindowsSystemFactory::getGraphicsManager(){
    return /*WindowsGraphicsManager::getInstance();*/ nullptr;
}
