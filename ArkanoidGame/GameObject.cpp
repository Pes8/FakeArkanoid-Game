#include "GameObject.h"

GameObject::GameObject() {
    m_oTexture = nullptr;
    m_oMesh = nullptr;
    collider = nullptr;
    scale = 1.0f;
}

GameObject::~GameObject() {
    
}