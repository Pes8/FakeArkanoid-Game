#pragma once

#include "GameObject.h"

struct Character : public GameObject {
    
    int m_iLives;
    
    Character();
    virtual ~Character();
    void CharGoLeft();
    void CharGoRight();
};
