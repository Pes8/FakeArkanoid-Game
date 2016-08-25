#pragma once

#include "GameObject.h"

struct Ball : public GameObject {
    Event<void> m_eBallOutsideScreen;
    Ball();
    virtual ~Ball();
    virtual void hit();
};
