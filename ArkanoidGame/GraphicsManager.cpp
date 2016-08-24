#include "GraphicsManager.h"

GraphicsManager * GraphicsManager::instance = nullptr;

void GraphicsManager::setScene(const std::vector<GameObject*> * _scene) {
    scene = _scene;
}
