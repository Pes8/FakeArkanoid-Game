#pragma once

#include "GraphicsInterface.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include "Types.h"


/* UTILITIES */
#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; }


using namespace DirectX;


//----------------------
// Structures
//----------------------
struct SimpleVertex{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
    XMFLOAT2 UV;
};


struct ConstantBuffer{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};



__declspec(align(16)) class D3DClass : public GraphicsInterface {
public:

    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fFar, float _fNear, void * _HWND);

    virtual bool run();
    virtual bool render(Scene * _scene);
    virtual bool shutdown();

    static GraphicsInterface * getInstance();


    virtual void setCamera(const Camera & _oCamera);

protected:
    D3DClass();

    virtual ~D3DClass();

    // XMMATRIX need a 16-bit alignment, so to garantue it on the heap memory I need to redefine the new and delete operators
    void* operator new(size_t i) {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p) {
        _mm_free(p);
    }

private:
    D3D_DRIVER_TYPE              m_driverType;
    D3D_FEATURE_LEVEL            m_featureLevel;
    ID3D11Device *               m_pd3dDevice;
    ID3D11DeviceContext *        m_pImmediateContext;
    IDXGISwapChain *             m_pSwapChain;
    ID3D11RenderTargetView *     m_pRenderTargetView;
    ID3D11VertexShader *         m_pVertexShader;
    ID3D11PixelShader *          m_pPixelShaderColor;
    ID3D11PixelShader *          m_pPixelShaderTexture;
    ID3D11SamplerState *         m_pSampleState;
    ID3D11Texture2D *            m_pTexture;
    ID3D11ShaderResourceView *   m_pTextureView;
    ID3D11InputLayout *          m_pVertexLayout;
    ID3D11Buffer *               m_pVertexBuffer;
    ID3D11Buffer *               m_pIndexBuffer;
    ID3D11Buffer *               m_pConstantBuffer;
    ID3D11Texture2D *            m_pDepthStencilBuffer;
    ID3D11DepthStencilState *    m_pDepthStencilState;
    ID3D11DepthStencilView *     m_pDepthStencilView;
    XMMATRIX                     m_World;
    XMMATRIX                     m_View;
    XMMATRIX                     m_Projection;

    bool loadMesh(GenericAsset * _object);
    bool loadTexture(GenericAsset::Texture * _tex);
};