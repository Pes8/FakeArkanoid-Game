#pragma once

#include "Types.h"
#include "ThirdPartyLib/stb_image.h"

struct GameObject : public ID{
    
    GameObject();
    virtual ~GameObject();
    virtual void hit() = 0;


    Texture * m_oTexture;
    Mesh * m_oMesh;
    Collider * collider ;

    Vector3 m_vPosition;
    Vector3 m_vRotation;
    Vector3 m_vVelocity;

    float scale;
};