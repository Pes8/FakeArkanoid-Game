#pragma once

#include "GenericAsset.h"

struct Block : public GenericAsset{
    Block();
    virtual void loadMesh();
    virtual void loadTexture();
    virtual ~Block();
};