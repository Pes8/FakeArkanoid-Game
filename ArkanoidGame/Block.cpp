#include "Block.h"
#include "GameManager.h"

Block::Block() {
    collider = new Collider();
    hitBeforeDestroy = 1;
}


Block::~Block() {

}

void Block::hit() {
    --hitBeforeDestroy;
    if (hitBeforeDestroy <= 0) {
        GameManager::getInstance()->OnBlockDestroyed(_MyID);
    }
}
