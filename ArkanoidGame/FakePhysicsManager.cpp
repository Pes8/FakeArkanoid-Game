#include "FakePhysicsManager.h"

FakePhysicsManager * FakePhysicsManager::instance = nullptr;

FakePhysicsManager * FakePhysicsManager::getInstance() {
    if (instance == nullptr) {
        instance = new FakePhysicsManager();
    }
    return instance;
}

void FakePhysicsManager::doPhysicUpdate() {
    
    moveObjects();

    
    for (GameObject * _obj : scene->getObjectList()) {
        bool coll = false;
        if (ball != _obj) { // do not check the same object
            coll = checkCollision(*ball, *_obj); // I'm interested only in collision beetwen ball and the scene
        }

        //That's why is called FAKE Physics Manager!
        if (coll) {
            if (_obj == player) {
                float _dist = (ball->m_vPosition.x - _obj->m_vPosition.x) / (_obj->scale);
                short _sign = (_dist < 0.0f) - (_dist > 0.0f);
                _dist = abs(_dist);
                float newAngle = PI_DIV_2 + (PI_DIV_6 + _dist * PI_DIV_3)*_sign; //radians

                Vector3 direction = { cos(newAngle),sin(newAngle), 0.0f };
                direction *= 0.15f;
                ball->m_vVelocity = direction;
            } else {
                
                //detecting direction
                float minY = _obj->m_vPosition.y - _obj->collider.sizes.y;
                float maxY = _obj->m_vPosition.y + _obj->collider.sizes.y;

                bool horizontal = ball->m_vPosition.y > minY && ball->m_vPosition.y < maxY;

                ball->m_vVelocity.x *= horizontal ? -1 : 1;
                ball->m_vVelocity.y *= horizontal ? 1 : -1;

            }
            break;
        }

    }
}

void FakePhysicsManager::initialization(Scene * _scene, GameObject * _player, GameObject * _ball) {
    this->scene = _scene;
    this->ball = _ball;
    this->player = _player;
}


bool FakePhysicsManager::checkCollision(const GameObject & a, const GameObject & b) {
    //check the X axis
    bool x = (std::abs(a.m_vPosition.x - b.m_vPosition.x) <= a.collider.sizes.x + b.collider.sizes.x);

    //check the Y axis
    bool y = (std::abs(a.m_vPosition.y - b.m_vPosition.y) <= a.collider.sizes.y + b.collider.sizes.y);

    //check the Z axis
    bool z = (std::abs(a.m_vPosition.z - b.m_vPosition.z) <= a.collider.sizes.z + b.collider.sizes.z);

    return x && y && z;
}


void FakePhysicsManager::moveObjects() {
    //I'm interested only to move Ball, Player is moved by The event called from input manager
    ball->m_vPosition += ball->m_vVelocity;
}

FakePhysicsManager::FakePhysicsManager() {

}
