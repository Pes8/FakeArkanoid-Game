#include "Wall.h"

Wall::Wall() {
    collider = new Collider();
}

Wall::~Wall() {
    SAFE_DELETE(collider);
}

void Wall::hit() {
    //do nothing
}
