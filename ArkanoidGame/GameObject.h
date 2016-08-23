#pragma once

#include "Types.h"
#include "stb_image.h"

struct GameObject {
    
    Texture * m_oTexture;

    Mesh * m_oMesh;

    Collider collider;

    Vector3 m_vPosition;
    Vector3 m_vRotation;
    Vector3 m_vVelocity;
    float scale = 1.0f;

    bool hasTexture = false;

    //virtual void loadMesh() = 0;
    //virtual void loadTexture() = 0;
    virtual ~GameObject();
};