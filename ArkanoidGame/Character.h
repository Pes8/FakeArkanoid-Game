#pragma once

#include "GameObject.h"

struct Character : public GameObject {
    
    int m_iLives;
    Event<void> m_ePlayerDeath;
    Character();
    virtual ~Character();
    void CharGoLeft();
    void CharGoRight();
    void LostLife();
    virtual void hit();
};
