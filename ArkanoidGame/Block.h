#pragma once

#include "GameObject.h"

struct Block : public GameObject{
    
    Block();
    virtual ~Block();
    virtual void hit();

    Event<int> m_eBlockDestroyed;
    unsigned short hitBeforeDestroy;
    bool m_bCracked;
};