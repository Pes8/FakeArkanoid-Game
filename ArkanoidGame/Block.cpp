#include "Block.h"
#include "GameManager.h"
#include "AssetsManager.h"

Block::Block() {
    collider = new Collider();
    hitBeforeDestroy = 1;
    m_eBlockDestroyed.par = _MyID;
    m_bCracked = false;
}


Block::~Block() {

}

void Block::hit() {
    --hitBeforeDestroy;
    
    if(!m_bCracked)
        m_oTexture = AssetsManager::getInstance()->getTexture(BLOCK_INDEX + 1);

    m_bCracked = true;

    if (hitBeforeDestroy <= 0) {
        m_eBlockDestroyed.fire();
        delete this; //suicide
    }
}
