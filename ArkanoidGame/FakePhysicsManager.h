#pragma once

#include "Types.h"
#include "GameObject.h"
#include "GameManager.h"
#include <cmath>


class FakePhysicsManager {
private:
    GameObject * player;
    GameObject * ball;

    bool checkCollision(const GameObject & a, const GameObject & b);
    void moveObjects();
    FakePhysicsManager();
    static FakePhysicsManager * instance;
public:
    static FakePhysicsManager * getInstance();
    void doPhysicUpdate();
    void initialization(const GameObject * _player, const GameObject * _ball);
};