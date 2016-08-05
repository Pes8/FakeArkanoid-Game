#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class ModelClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();