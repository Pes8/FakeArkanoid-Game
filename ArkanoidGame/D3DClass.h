#pragma once
//These works only on VC++
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include "GraphicsInterface.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <DirectXMath.h>

using namespace DirectX;

__declspec(align(16)) class D3DClass : public GraphicsInterface {
public:

    virtual bool initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd = nullptr);
    virtual bool run();
    virtual bool render();
    virtual bool shutdown();

    static GraphicsInterface * getInstance();
protected:
    D3DClass();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    void GetProjectionMatrix(XMMATRIX& _matrix);
    void GetWorldMatrix(XMMATRIX& _matrix);
    void GetOrthoMatrix(XMMATRIX& _matrix);

    void GetVideoCardInfo(char*, int&);

    virtual ~D3DClass();

    // XMMATRIX need a 16-bit alignment, so to garantue it on the heap memory I need to redefine the new and delete operators
    void* operator new(size_t i) {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p) {
        _mm_free(p);
    }

private:
    
    unsigned int m_videoCardMemory;
    IDXGISwapChain * m_swapChain;
    ID3D11Device * m_device;
    ID3D11DeviceContext * m_deviceContext;
    ID3D11RenderTargetView * m_renderTargetView;
    ID3D11Texture2D * m_depthStencilBuffer;
    ID3D11DepthStencilState * m_depthStencilState;
    ID3D11DepthStencilView * m_depthStencilView;
    ID3D11RasterizerState * m_rasterState;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_worldMatrix;
    XMMATRIX m_orthoMatrix;
    char m_videoCardDescription[128];
    bool m_vsync_enabled;
};