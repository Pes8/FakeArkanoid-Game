#include "Block.h"

Block::Block() {
    //TODO: think about read the verticies from a mesh or similar

    //right now I'll build manually vertices
    m_iVertexCount = 3;
    m_iIndexCount = 3;

    m_aoVertices = new Vertex[m_iVertexCount];
    m_alIndices = new unsigned long[m_iIndexCount];

    // Load the vertex array with data.
    m_aoVertices[0].position = { -1.0f, -1.0f, 0.0f };  // Bottom left.
    m_aoVertices[0].color = { 0.0f, 1.0f, 0.0f, 1.0f };

    m_aoVertices[1].position = { 0.0f, 1.0f, 0.0f };  // Top middle.
    m_aoVertices[1].color = { 0.0f, 1.0f, 0.0f, 1.0f };

    m_aoVertices[2].position = {1.0f, -1.0f, 0.0f};  // Bottom right.
    m_aoVertices[2].color = { 0.0f, 1.0f, 0.0f, 1.0f };

    // Load the index array with data.
    m_alIndices[0] = 0;  // Bottom left.
    m_alIndices[1] = 1;  // Top middle.
    m_alIndices[2] = 2;  // Bottom right.
}
