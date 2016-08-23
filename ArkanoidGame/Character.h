#pragma once

#include "GameObject.h"

struct Character : public GameObject {
    Character();
    //virtual void loadMesh();
    //virtual void loadTexture();
    virtual ~Character();
    void CharGoLeft();
    void CharGoRight();
};
