#pragma once

#include "Types.h"
#include "GameObject.h"
#include "GameManager.h"
#include <cmath>


class FakePhysicsManager {
public:
    static FakePhysicsManager * getInstance();
    void doPhysicUpdate();
    void initialization(const GameObject * _player, const GameObject * _ball);

private:

    FakePhysicsManager();
    bool checkCollision(const GameObject & a, const GameObject & b) const;
    void moveObjects();
    bool isInside(const GameObject & _obj) const;

    static FakePhysicsManager * instance;
    GameObject * player;
    GameObject * ball;

};