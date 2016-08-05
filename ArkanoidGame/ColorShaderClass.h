#pragma once

//These works only on VC++
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;


__declspec(align(16)) class ColorShaderClass {
public:
    ColorShaderClass();
    ColorShaderClass(const ColorShaderClass&);
    ~ColorShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix);

    // XMMATRIX need a 16-bit alignment, so to garantue it on the heap memory I need to redefine the new and delete operators
    void* operator new(size_t i) {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p) {
        _mm_free(p);
    }

private:
    struct MatrixBufferType {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;

    bool InitializeShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCSTR);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX &);
    void RenderShader(ID3D11DeviceContext*, int);
};