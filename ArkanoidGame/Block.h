#pragma once

#include "Types.h"

struct Block {
    Block();
    unsigned int m_iVertexCount = 0;
    unsigned int m_iIndexCount = 0;
    float m_afPosition[3] = { 0.0f, 0.0f, 0.0f };
    VertexInfo * m_aoVertices = nullptr;
    unsigned long * m_alIndices = nullptr;
};