#pragma once

#include "GameObject.h"

struct Block : public GameObject{
    Block();
    virtual void loadMesh();
    virtual void loadTexture();
    virtual ~Block();
};