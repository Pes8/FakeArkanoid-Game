#pragma once

#include "GameObject.h"

struct Block : public GameObject{
    int hitBeforeDestroy;
    Block();
    virtual ~Block();
    virtual void hit();
};