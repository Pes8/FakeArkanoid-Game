#include "D3DClass.h"


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
    m_pPixelShader = nullptr;
    m_pVertexLayout = nullptr;
    m_pVertexBuffer = nullptr;
    m_pIndexBuffer = nullptr;
    m_pConstantBuffer = nullptr;
}

D3DClass::~D3DClass() {

}

bool D3DClass::loadMesh(GenericAsset * _object) {
    HRESULT result;

    m_World = XMMatrixIdentity();

    unsigned int totalVertices = _object->m_iVertexCount;
    unsigned int _numDrawVertex = _object->m_iIndexCount;

    SimpleVertex * vertices = new SimpleVertex[totalVertices];
    memcpy(vertices, _object->m_aoVertices, sizeof(VertexInfo) * totalVertices);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * totalVertices;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    result = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
    if (FAILED(result))
        return false;

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    WORD * indices = new WORD[_numDrawVertex];
    memcpy(indices, _object->m_alIndices, sizeof(WORD) * _numDrawVertex);
    
    // Create index buffer

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * _numDrawVertex;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
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
    m_World *= XMMatrixTranslation(_object->m_vPosition.x, _object->m_vPosition.y, _object->m_vPosition.z);
    m_World *= XMMatrixRotationRollPitchYaw(_object->m_vRotation.x, _object->m_vRotation.y, _object->m_vRotation.z);
    m_World *= XMMatrixScaling(_object->scale, _object->scale, _object->scale);

    // Release the arrays now that the vertex and index buffers have been created and loaded.
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

bool D3DClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fFar, float _fNear, void * _HWND) {
    
    HRESULT result = true;
    IDXGIFactory * factory;
    IDXGIAdapter * adapter;
    IDXGIOutput * adapterOutput;
    unsigned int numDisplayModes, VSyncNumerator, VSyncDenominator;
    DXGI_MODE_DESC * displayModeList;

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

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

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
    result = CompileShaderFromFile(L"SimpleShader.fx", "VS", "vs_4_0", &pVSBlob);
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
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    result = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(result))
        return false;

    // Set the input layout
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    result = CompileShaderFromFile(L"SimpleShader.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(result)) {
        MessageBox(NULL,
                   "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return false;
    }

    // Create the pixel shader
    result = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
    pPSBlob->Release();
    if (FAILED(result))
        return false;

    

    // Initialize the world matrix
    m_World = XMMatrixIdentity();

    // Initialize the projection matrix
    //m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, _iScreenWidth / (FLOAT)_iScreenHeight, _fNear, _fFar);
    m_Projection = XMMatrixOrthographicLH(20.0, 20.0, _fNear, _fFar);
    return true;

}

bool D3DClass::run() {
    return true;
}

bool D3DClass::render(Scene * _scene) {
    
    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    //Put all objects in one array and creates buffers
    //int numVertices = 0;
    /*bool res = renderObjectsTogheter(_scene, numVertices);*/

    for (GenericAsset * _asset : _scene->getObjectList()) {
        bool res = loadMesh(_asset);
        if (!res)
            return false;

        //
        // Update variables
        //
        ConstantBuffer cb;
        cb.mWorld = XMMatrixTranspose(m_World);
        cb.mView = XMMatrixTranspose(m_View);
        cb.mProjection = XMMatrixTranspose(m_Projection);
        m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);


        //
        // Renders a triangle
        //
        m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
        m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
        m_pImmediateContext->DrawIndexed(_asset->m_iIndexCount, 0, 0);


        SAFE_RELEASE(m_pIndexBuffer);
        SAFE_RELEASE(m_pVertexBuffer);
        SAFE_RELEASE(m_pConstantBuffer);
    }
    

    //
    // Present our back buffer to our front buffer
    //
    m_pSwapChain->Present(0, 0);
    
    

    return true;
}

bool D3DClass::shutdown() {

    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);

    if (m_pImmediateContext) {
        m_pImmediateContext->ClearState();
        m_pImmediateContext->Flush();
    }

    SAFE_RELEASE(m_pConstantBuffer);

    SAFE_RELEASE(m_pVertexBuffer);

    SAFE_RELEASE(m_pIndexBuffer);

    SAFE_RELEASE(m_pVertexLayout);

    SAFE_RELEASE(m_pVertexShader);

    SAFE_RELEASE(m_pPixelShader);

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

    return true;
}

GraphicsInterface * D3DClass::getInstance() {
    if (D3DClass::instance == nullptr) {
        D3DClass::instance = new D3DClass();
    }
    return D3DClass::instance;
}

void D3DClass::setCamera(const Camera & _oCamera) {
    
    XMVECTOR Eye = XMVectorSet(_oCamera.m_afPosition.x, _oCamera.m_afPosition.y, _oCamera.m_afPosition.z, 0.0f);
    XMVECTOR At = XMVectorSet(_oCamera.m_afLookAt.x, _oCamera.m_afLookAt.y, _oCamera.m_afLookAt.z, 0.0f);
    XMVECTOR Up = XMVectorSet(_oCamera.m_afUp.x, _oCamera.m_afUp.y, _oCamera.m_afUp.z, 0.0f);
    m_View = XMMatrixLookAtLH(Eye, At, Up);
}
