#include "Block.h"

Block::Block() {
    
}

void Block::loadMesh() {
    
    m_iVertexCount = 8;
    m_iIndexCount = 36;
    
    m_aoVertices = new VertexInfo[m_iVertexCount];
    m_aoVertices[0] = { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
    m_aoVertices[1] = { 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f };
    m_aoVertices[2] = { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    m_aoVertices[3] = { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };
    m_aoVertices[4] = { -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
    m_aoVertices[5] = { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    m_aoVertices[6] = { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    m_aoVertices[7] = { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };


    m_alIndices = new unsigned short[m_iIndexCount]{
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
}

Block::~Block() {
    
}
