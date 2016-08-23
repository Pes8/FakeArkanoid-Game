#pragma once

#include "Types.h"
#include "GameObject.h"
#include "Scene.h"
#include <cmath>
#include "Scene.h"

class FakePhysicsManager {
private:
    GameObject * player;
    GameObject * ball;
    Scene * scene;

    bool checkCollision(const GameObject & a, const GameObject & b);
    void moveObjects();
    FakePhysicsManager();
    static FakePhysicsManager * instance;
public:
    static FakePhysicsManager * getInstance();
    void doPhysicUpdate();
    void initialization(Scene *_scene, GameObject * _player, GameObject * _ball);
};