#pragma once

#include "Types.h"


struct GenericAsset {
    
    VertexInfo * m_aoVertices = nullptr;
    Vector3 m_vPosition;
    Vector3 m_vRotation;
    float scale = 1.0f;
    unsigned int m_iVertexCount = 0;
    unsigned int m_iIndexCount = 0;
    unsigned short * m_alIndices = nullptr;

    virtual void loadMesh() = 0;
    virtual ~GenericAsset();
};