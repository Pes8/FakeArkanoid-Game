#include "Character.h"

Character::Character() {
    collider = new Collider();
    collider->active = true;
    collider->sizes = { PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z };
    collider->type = Collider::AABB;

    m_iLives = PLAYER_LIVES;

    m_vVelocity = {0,0,0};
}

Character::~Character() {

}

void Character::CharGoLeft() {
    if (m_vPosition.x - collider->sizes.x - PLAYER_VELOCITY > -(CAMERA_ORTO_WIDTH - WALL_THICKNESS_X) / 2)
        m_vVelocity.x -= PLAYER_VELOCITY;
}

void Character::CharGoRight() {
    if(m_vPosition.x + collider->sizes.x  + PLAYER_VELOCITY < (CAMERA_ORTO_WIDTH - WALL_THICKNESS_X) / 2)
        m_vVelocity.x += PLAYER_VELOCITY;
}

void Character::LostLife() {
    --m_iLives;

    if (m_iLives <= 0) {
        m_ePlayerDeath.fire();
        m_ePlayerDeath.subscriptions.clear();
    }
}

void Character::hit() {
    //Do nothing
}
