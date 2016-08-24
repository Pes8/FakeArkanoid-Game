#pragma once

#include "Types.h"
#include "stb_image.h"

struct GameObject : public ID{
    
    Texture * m_oTexture;
    Mesh * m_oMesh;
    Collider * collider ;

    Vector3 m_vPosition;
    Vector3 m_vRotation;
    Vector3 m_vVelocity;
    float scale;
    virtual void hit() = 0;
    GameObject();
    virtual ~GameObject();
};