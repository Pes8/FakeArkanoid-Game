#include "Ball.h"

Ball::Ball() {
    collider = new Collider();
}

Ball::~Ball() {
    SAFE_DELETE(collider);
}

void Ball::hit() {
    //do nothing
}
