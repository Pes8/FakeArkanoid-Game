#include "Block.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Block::Block() {
    
}

void Block::loadMesh() {
    
    m_iVertexCount = 8;
    m_iIndexCount = 36;
    
    m_aoVertices = new VertexInfo[m_iVertexCount];
    m_aoVertices[0] = { -0.5f, 0.5f, -0.5f, m_oTexture.defaultColor, 0.0f, 1.0f };
    m_aoVertices[1] = { 0.5f, 0.5f, -0.5f,  m_oTexture.defaultColor, 0.0f, 0.0f };
    m_aoVertices[2] = { 0.5f, 0.5f, 0.5f,  m_oTexture.defaultColor, 1.0f, 0.0f };
    m_aoVertices[3] = { -0.5f, 0.5f, 0.5f,  m_oTexture.defaultColor, 1.0f, 1.0f };
    m_aoVertices[4] = { -0.5f, -0.5f, -0.5f,  m_oTexture.defaultColor, 1.0f, 1.0f };
    m_aoVertices[5] = { 0.5f, -0.5f, -0.5f,  m_oTexture.defaultColor, 1.0f, 0.0f };
    m_aoVertices[6] = { 0.5f, -0.5f, 0.5f,  m_oTexture.defaultColor, 0.0f, 0.0f };
    m_aoVertices[7] = { -0.5f, -0.5f, 0.5f,  m_oTexture.defaultColor, 0.0f, 1.0f };


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

void Block::loadTexture() {
    m_oTexture.data = stbi_load("./Data/Texture/crate.jpg", &m_oTexture.width, &m_oTexture.height, &m_oTexture.channels, m_oTexture.channels);
    if(m_oTexture.data)
        hasTexture = true;
}

Block::~Block() {
    stbi_image_free(m_oTexture.data);
}
