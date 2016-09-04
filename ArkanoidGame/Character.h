#pragma once

#include "GameObject.h"

struct Character : public GameObject {
    
    Character();
    virtual ~Character();
    void CharGoLeft();
    void CharGoRight();
    void LostLife();
    virtual void hit();

    Event<void> m_ePlayerDeath;
    unsigned short m_iLives;
};
