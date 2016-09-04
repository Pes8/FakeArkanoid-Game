#pragma once

#include "GameObject.h"

struct Ball : public GameObject {
    
    Ball();
    virtual ~Ball();
    virtual void hit();

    Event<void> m_eBallOutsideScreen;
};
