#pragma once

#include "GameObject.h"

struct Wall : public GameObject {
    Wall();
    virtual ~Wall();
    virtual void hit();
};