#pragma once

#include "GameObject.h"

struct Block : public GameObject{
    int hitBeforeDestroy;
    Block();
    virtual ~Block();
    virtual void hit();
    Event<int> m_eBlockDestroyed;
    bool m_bCracked;
};