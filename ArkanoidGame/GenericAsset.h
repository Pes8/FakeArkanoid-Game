#pragma once

#include "Types.h"


struct GenericAsset {
    
    VertexInfo * m_aoVertices = nullptr;
    struct Texture {
        unsigned char * data = nullptr;
        int width = 0;
        int height = 0;
        int channels = 4;
        Vector4 defaultColor = {0.6f, 0.3f, 0.2f, 1.0f};
    } m_oTexture;
    
    Vector3 m_vPosition;
    Vector3 m_vRotation;
    float scale = 1.0f;
    unsigned int m_iVertexCount = 0;
    unsigned int m_iIndexCount = 0;
    unsigned short * m_alIndices = nullptr;

    bool hasTexture = false;

    virtual void loadMesh() = 0;
    virtual void loadTexture() = 0;
    virtual ~GenericAsset();
};