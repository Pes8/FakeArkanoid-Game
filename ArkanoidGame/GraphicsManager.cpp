#include "GraphicsManager.h"

GraphicsManager * GraphicsManager::instance = nullptr;

void GraphicsManager::setScene(Scene * _scene) {
    scene = _scene;
}
