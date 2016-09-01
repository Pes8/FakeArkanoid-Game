#include "D3DClass.h"
#include "WindowsGraphicsManager.h"


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef GPU_DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr)) {
        //MessageBox(NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return hr;
    }
    SAFE_RELEASE(pErrorBlob);

    return S_OK;
}


D3DClass::D3DClass() {
    m_driverType = D3D_DRIVER_TYPE_NULL;
    m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    m_pd3dDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pSwapChain = nullptr;
    m_pRenderTargetView = nullptr;
    m_pVertexShader = nullptr;
    m_pPixelShaderColor = nullptr;
    m_pPixelShaderTexture = nullptr;
    m_pVertexLayout = nullptr;
    m_pVertexBuffer = nullptr;
    m_pIndexBuffer = nullptr;
    m_pConstantBuffer = nullptr;
    m_pDepthStencilBuffer = nullptr;
    m_pDepthStencilState = nullptr;
    m_pDepthStencilView = nullptr;
    m_pSampleState = nullptr;
    m_pTextureView = nullptr;
    m_pTexture = nullptr;

    m_pD2DFactory = nullptr;
    m_pBackBufferRT = nullptr;
    m_pBackBufferBrush = nullptr;
    m_pDWriteFactory = nullptr;
    m_pTextFormatM = nullptr;
}

D3DClass::~D3DClass() {

}

bool D3DClass::loadMesh(GameObject * _object) {
    HRESULT result;

    m_World = XMMatrixIdentity();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VertexInfo) * _object->m_oMesh->m_iVertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = _object->m_oMesh->m_aoVertices;
    result = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
    if (FAILED(result))
        return false;

    // Set vertex buffer
    UINT stride = sizeof(VertexInfo);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    
    // Create index buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned short) * _object->m_oMesh->m_iIndexCount;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = _object->m_oMesh->m_alIndices;
    result = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
    if (FAILED(result))
        return false;

    // Set index buffer
    m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create the constant buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    result = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
    if (FAILED(result))
        return false;

    //Update world matrix
    m_World *= XMMatrixScaling(_object->scale, _object->scale, _object->scale);
    m_World *= XMMatrixTranslation(_object->m_vPosition.x, _object->m_vPosition.y, _object->m_vPosition.z);
    m_World *= XMMatrixRotationRollPitchYaw(_object->m_vRotation.x, _object->m_vRotation.y, _object->m_vRotation.z);


    return true;
}

bool D3DClass::loadTexture(Texture * _tex) {
    
    HRESULT hResult;
    
    // Setup the description of the texture.
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Height = _tex->height;
    textureDesc.Width = _tex->width;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    // Create the empty texture.
    hResult = m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &m_pTexture);
    if (FAILED(hResult)) {
        return false;
    }

    // Set the row pitch of the targa image data.
    unsigned int rowPitch = (_tex->width * 4) * sizeof(unsigned char);

    // Copy the targa image data into the texture.
    m_pImmediateContext->UpdateSubresource(m_pTexture, 0, NULL, _tex->data, rowPitch, 0);


    // Setup the shader resource view description.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    // Create the shader resource view for the texture.
    hResult = m_pd3dDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
    if (FAILED(hResult)) {
        return false;
    }

    // Generate mipmaps for this texture.
    m_pImmediateContext->GenerateMips(m_pTextureView);
    SAFE_RELEASE(m_pTexture);
    return true;
}

bool D3DClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fFar, float _fNear, void * _HWND) {
    
    HRESULT result = true;
    IDXGIFactory * factory;
    IDXGIAdapter * adapter;
    IDXGIOutput * adapterOutput;
    unsigned int numDisplayModes, VSyncNumerator, VSyncDenominator;
    DXGI_MODE_DESC * displayModeList;

    VSyncNumerator = 0;
    VSyncDenominator = 1;

    // Create a DirectX graphics interface factory.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result)) {
        return false;
    }

    // Use the factory to create an adapter for the primary graphics interface (video card).
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result)) {
        return false;
    }

    // Enumerate the primary adapter output (monitor).
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result)) {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL);
    if (FAILED(result)) {
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination.
    displayModeList = new DXGI_MODE_DESC[numDisplayModes];
    if (!displayModeList) {
        return false;
    }

    // Now fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
    if (FAILED(result)) {
        return false;
    }

    // Now go tresultough all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for (unsigned int i = 0; i < numDisplayModes; ++i) {
        if (displayModeList[i].Width == _iScreenWidth) {
            if (displayModeList[i].Height == _iScreenHeight) {
                VSyncNumerator = displayModeList[i].RefreshRate.Numerator;
                VSyncDenominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // Release the display mode list.
    delete[] displayModeList;
    displayModeList = nullptr;

    // Release the adapter output.
    adapterOutput->Release();
    adapterOutput = nullptr;

    // Release the adapter.
    adapter->Release();
    adapter = nullptr;

    // Release the factory.
    factory->Release();
    factory = nullptr;


    UINT createDeviceFlags = 0;
#ifdef GPU_DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = _iScreenWidth;
    swapChainDesc.BufferDesc.Height = _iScreenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // Set the refresh rate of the back buffer.
    if (_bVSyncEnabled) {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = VSyncNumerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = VSyncDenominator;
    } else {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = reinterpret_cast<HWND>(_HWND);
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !_bFullscreen;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        m_driverType = driverTypes[driverTypeIndex];
        result = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
        if (SUCCEEDED(result))
            break;
    }
    if (FAILED(result))
        return false;

    
    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(result))
        return false;

    result = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(result))
        return false;

    
    /***** START 2D STUFF *****/

    // Create a Direct2D factory.
    result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(result))
        return false;

    IDXGISurface * pD2DBackBuffer;
    // Get a surface in the swap chain
    result = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pD2DBackBuffer));
    if (FAILED(result))
        return false;

    // Create the DXGI Surface Render Target.
    FLOAT dpiX;
    FLOAT dpiY;
    m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

    D2D1_RENDER_TARGET_PROPERTIES props =  D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
    result = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pD2DBackBuffer, &props, &m_pBackBufferRT);
    if (FAILED(result))
        return false;

    // Create a DirectWrite factory.
    result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown **>(&m_pDWriteFactory));

    // Create a DirectWrite text format object.
    result = m_pDWriteFactory->CreateTextFormat(UI_FONT_NAME, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, UI_FONT_SIZE_S, L"", &m_pTextFormatS);
    result = m_pDWriteFactory->CreateTextFormat(UI_FONT_NAME, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, UI_FONT_SIZE_M, L"", &m_pTextFormatM );
    result = m_pDWriteFactory->CreateTextFormat(UI_FONT_NAME, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, UI_FONT_SIZE_L, L"", &m_pTextFormatL);

    // Center the text horizontally and vertically.
    m_pTextFormatS->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_pTextFormatS->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    m_pTextFormatM->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_pTextFormatM->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    m_pTextFormatL->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_pTextFormatL->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    
    m_pBackBufferRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White, 0.85f),
        &m_pBackBufferBrush
    );

    /***** END 2D STUFF *****/


    // Initialize the description of the depth buffer.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = _iScreenWidth;
    depthBufferDesc.Height = _iScreenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    result = m_pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
    if (FAILED(result)) {
        return false;
    }


    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    result = m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
    if (FAILED(result)) {
        return false;
    }

    // Set the depth stencil state.
    m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
    if (FAILED(result)) {
        return false;
    }

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_iScreenWidth;
    vp.Height = (FLOAT)_iScreenHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    result = CompileShaderFromFile(L"TextureShader.hlsl", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(result)) {
        MessageBox(NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return false;
    }

    // Create the vertex shader
    result = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
    if (FAILED(result)) {
        pVSBlob->Release();
        return false;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    result = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(result))
        return false;

    // Set the input layout
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

    // Compile the pixel shader - Color Entry Point
    ID3DBlob* pPSBlob = NULL;
    result = CompileShaderFromFile(L"TextureShader.hlsl", "PS_Color", "ps_4_0", &pPSBlob);
    if (FAILED(result)) {
        MessageBox(NULL,
                   "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return false;
    }

    // Create the pixel shader - Color Entry Point
    result = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShaderColor);
    pPSBlob->Release();
    if (FAILED(result))
        return false;


    // Compile the pixel shader - Texture Entry Point
    ID3DBlob* pPSBlob2 = NULL;
    result = CompileShaderFromFile(L"TextureShader.hlsl", "PS_Texture", "ps_4_0", &pPSBlob2);
    if (FAILED(result)) {
        MessageBox(NULL,
                   "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return false;
    }

    // Create the pixel shader - Texture Entry Point
    result = m_pd3dDevice->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), NULL, &m_pPixelShaderTexture);
    pPSBlob2->Release();
    if (FAILED(result))
        return false;
    

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    result = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pSampleState);
    if (FAILED(result)) {
        return false;
    }



    // Initialize the world matrix
    m_World = XMMatrixIdentity();

    // Initialize the projection matrix
    //m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, _iScreenWidth / (FLOAT)_iScreenHeight, _fNear, _fFar);
    m_Projection = XMMatrixOrthographicLH(CAMERA_ORTO_WIDTH, CAMERA_ORTO_HEIGHT, _fNear, _fFar);

    return true;
}

bool D3DClass::run() {
    return true;
}

bool D3DClass::render(const std::vector<GameObject*> * _scene, const std::vector<UIText*> * _ui) {
    
    // Clear the back buffer and the depth buffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.1f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
   

    for (GameObject * _asset : * _scene) {
        bool res = loadMesh(_asset);
        if (!res)
            return false;

        // Update variables
        ConstantBuffer cb;
        cb.mWorld = XMMatrixTranspose(m_World);
        cb.mView = XMMatrixTranspose(m_View);
        cb.mProjection = XMMatrixTranspose(m_Projection);
        m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);


        // Renders a triangle
        m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
        m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        if (_asset->m_oTexture && _asset->m_oTexture->data) {
            loadTexture(_asset->m_oTexture);
            m_pImmediateContext->PSSetShader(m_pPixelShaderTexture, NULL, 0);
            // Set shader texture resource in the pixel shader.
            m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureView);
        } else {
            m_pImmediateContext->PSSetShader(m_pPixelShaderColor, NULL, 0);
        }

        m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampleState);

        m_pImmediateContext->DrawIndexed(_asset->m_oMesh->m_iIndexCount, 0, 0);

        SAFE_RELEASE(m_pTextureView);
        SAFE_RELEASE(m_pIndexBuffer);
        SAFE_RELEASE(m_pVertexBuffer);
        SAFE_RELEASE(m_pConstantBuffer);
    }

    m_pBackBufferRT->BeginDraw();
    //D2D1_SIZE_F _2DSize = m_pBackBufferRT->GetSize();
    for (UIText * _text : *_ui) {
        switch (_text->m_eType) {
            case UIText::SMALL:
                m_pBackBufferRT->DrawText(_text->m_pText, _text->m_iSize, m_pTextFormatS, D2D1::RectF(_text->m_vStartRectPosition.u, _text->m_vStartRectPosition.v, _text->m_vEndRectPosition.u, _text->m_vEndRectPosition.v), m_pBackBufferBrush);
                break;
            case UIText::LARGE:
                m_pBackBufferRT->DrawText(_text->m_pText, _text->m_iSize, m_pTextFormatL, D2D1::RectF(_text->m_vStartRectPosition.u, _text->m_vStartRectPosition.v, _text->m_vEndRectPosition.u, _text->m_vEndRectPosition.v), m_pBackBufferBrush);
                break;
            default:
                m_pBackBufferRT->DrawText(_text->m_pText, _text->m_iSize, m_pTextFormatM, D2D1::RectF(_text->m_vStartRectPosition.u, _text->m_vStartRectPosition.v, _text->m_vEndRectPosition.u, _text->m_vEndRectPosition.v), m_pBackBufferBrush);
                break;
        }
    }
    m_pBackBufferRT->EndDraw();

    // Present our back buffer to our front buffer - nice to meet you!
    m_pSwapChain->Present(0, 0);

    return true;
}

bool D3DClass::shutdown() {

    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);

    SAFE_RELEASE(m_pDepthStencilBuffer);
    SAFE_RELEASE(m_pDepthStencilState);
    SAFE_RELEASE(m_pDepthStencilView);

    if (m_pImmediateContext) {
        m_pImmediateContext->ClearState();
        m_pImmediateContext->Flush();
    }

    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexLayout);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pPixelShaderColor);
    SAFE_RELEASE(m_pPixelShaderTexture);
    SAFE_RELEASE(m_pSampleState);
    SAFE_RELEASE(m_pTextureView);
    SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pRenderTargetView);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pImmediateContext);

#ifdef GPU_DEBUG
    ID3D11Debug* DebugDevice = nullptr;
    HRESULT Result = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&DebugDevice));
    Result = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    DebugDevice->Release();
#endif // DEBUG

    SAFE_RELEASE(m_pd3dDevice);

    SAFE_RELEASE(m_pD2DFactory);
    SAFE_RELEASE(m_pBackBufferRT);
    SAFE_RELEASE(m_pBackBufferBrush);
    SAFE_RELEASE(m_pDWriteFactory);
    SAFE_RELEASE(m_pTextFormatM);
    SAFE_RELEASE(m_pTextFormatS);
    SAFE_RELEASE(m_pTextFormatM);
    SAFE_RELEASE(m_pTextFormatL);

    return true;
}

GraphicsInterface * D3DClass::getInstance() {
    if (D3DClass::instance == nullptr) {
        D3DClass::instance = new D3DClass();
    }
    return D3DClass::instance;
}

void * D3DClass::getHandle() {
    return m_pd3dDevice;
}

void D3DClass::setCamera(const Camera & _oCamera) {
    
    XMVECTOR Eye = XMVectorSet(_oCamera.m_afPosition.x, _oCamera.m_afPosition.y, _oCamera.m_afPosition.z, 0.0f);
    XMVECTOR At = XMVectorSet(_oCamera.m_afLookAt.x, _oCamera.m_afLookAt.y, _oCamera.m_afLookAt.z, 0.0f);
    XMVECTOR Up = XMVectorSet(_oCamera.m_afUp.x, _oCamera.m_afUp.y, _oCamera.m_afUp.z, 0.0f);
    m_View = XMMatrixLookAtLH(Eye, At, Up);
}
