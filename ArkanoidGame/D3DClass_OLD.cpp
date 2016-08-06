/*

D3DClass::D3DClass() {
    m_swapChain = nullptr;
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_renderTargetView = nullptr;
    m_depthStencilBuffer = nullptr;
    m_depthStencilState = nullptr;
    m_depthStencilView = nullptr;
    m_rasterState = nullptr;
}

D3DClass::~D3DClass() {

}

bool D3DClass::initializeBuffers(Vertex * _vertices, unsigned long _vertexNumber, unsigned long * _indeces, unsigned long _indexNumber) {
    
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    
    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexNumber;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = _vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexNumber;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = _indeces;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
        return false;
    }

    //Ora potrei cancellare i due array (_vertices, _indeces) per recuperare memoria ... conviene farlo ? 

    return true;
}

void D3DClass::renderBuffers() {
    unsigned int stride;
    unsigned int offset;

    // Set vertex buffer stride and offset.
    stride = sizeof(Vertex);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3DClass::shutdownBuffers() {
    // Release the index buffer.
    if (m_indexBuffer) {
        m_indexBuffer->Release();
        m_indexBuffer = nullptr;
    }

    // Release the vertex buffer.
    if (m_vertexBuffer) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }

}

bool D3DClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd) {

    HRESULT result;
    IDXGIFactory * factory;
    IDXGIAdapter * adapter;
    IDXGIOutput * adapterOutput;
    unsigned int numDisplayModes, VSyncNumerator, VSyncDenominator, stringLength;
    DXGI_MODE_DESC * displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    // Set the feature level to DirectX 11.
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    ID3D11Texture2D * backBufferPtr;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC rasterDesc;
    D3D11_VIEWPORT viewport;
    float fieldOfView, screenAspect;

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

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for (unsigned int i = 0; i < numDisplayModes; ++i) {
        if (displayModeList[i].Width == _iScreenWidth) {
            if (displayModeList[i].Height == _iScreenHeight) {
                VSyncNumerator = displayModeList[i].RefreshRate.Numerator;
                VSyncDenominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // Get the adapter (video card) description.
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result)) {
        return false;
    }

    // Store the dedicated video card memory in megabytes.
    m_videoCardMemory = adapterDesc.DedicatedVideoMemory / 1024 / 1024;

    // Convert the name of the video card to a character array and store it.
    error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0) {
        return false;
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

    // Initialize the swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

    // Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = _iScreenWidth;
    swapChainDesc.BufferDesc.Height = _iScreenHeight;

    // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the refresh rate of the back buffer.
    if (_bVSyncEnabled) {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = VSyncNumerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = VSyncDenominator;
    } else {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window to render to.
    swapChainDesc.OutputWindow = reinterpret_cast<HWND>(m_hwnd);

    // Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Set to full screen or windowed mode.
    swapChainDesc.Windowed = !_bFullscreen;

    // Set the scan line ordering and scaling to unspecified.
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard the back buffer contents after presenting.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Don't set the advanced flags.
    swapChainDesc.Flags = 0;

    // Create the swap chain, Direct3D device, and Direct3D device context.
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
                                           D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
    if (FAILED(result)) {
        return false;
    }

    // Get the pointer to the back buffer.
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result)) {
        return false;
    }

    // Create the render target view with the back buffer pointer.
    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(result)) {
        return false;
    }

    // Release pointer to the back buffer as we no longer need it.
    backBufferPtr->Release();
    backBufferPtr = nullptr;

    // Initialize the description of the depth buffer.
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

    // Create the texture for the depth buffer (stencil buffer) using the filled out description.
    result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

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
    result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if (FAILED(result)) {
        return false;
    }

    // Set the depth stencil state.
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

    // Initailze the depth stencil view.
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(result)) {
        return false;
    }

    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state from the description we just filled out.
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(result)) {
        return false;
    }

    // Now set the rasterizer state.
    m_deviceContext->RSSetState(m_rasterState);

    // Setup the viewport for rendering.
    viewport.Width = (float)_iScreenWidth;
    viewport.Height = (float)_iScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create the viewport.
    m_deviceContext->RSSetViewports(1, &viewport);

    // Setup the projection matrix.
    fieldOfView = XM_PI / 4.0f;
    screenAspect = (float)_iScreenWidth / (float)_iScreenHeight;

    // Create the projection matrix for 3D rendering.
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, _fScreenNear, _fScreenDepth);

    // Initialize the world matrix to the identity matrix.
    m_worldMatrix = XMMatrixIdentity();

    // Create an orthographic projection matrix for 2D rendering.
    m_orthoMatrix = XMMatrixOrthographicLH((float)_iScreenWidth, (float)_iScreenHeight, _fScreenNear, _fScreenDepth);

    return true;
}

bool D3DClass::loadAsset(Block * _block) {
    return initializeBuffers(_block->m_aoVertices, _block->m_iVertexCount, _block->m_alIndices, _block->m_iIndexCount);
}

bool D3DClass::run() {

    return true;
}

bool D3DClass::render() {
    /*
    // Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_D3D->EndScene();
    */

    float color[4];

    // Setup the color to clear the buffer to.
    color[0] = 0.2f;
    color[1] = 0.2f;
    color[2] = 0.8f;
    color[3] = 1.0f;

    // Clear the back buffer.
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    // Clear the depth buffer.
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Present the back buffer to the screen since rendering is complete.
    m_swapChain->Present(m_vsync_enabled, 0);

    return true;
}

bool D3DClass::shutdown() {

    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
    if (m_swapChain) {
        m_swapChain->SetFullscreenState(false, NULL);
    }

    if (m_rasterState) {
        m_rasterState->Release();
        m_rasterState = nullptr;
    }

    if (m_depthStencilView) {
        m_depthStencilView->Release();
        m_depthStencilView = nullptr;
    }

    if (m_depthStencilState) {
        m_depthStencilState->Release();
        m_depthStencilState = nullptr;
    }

    if (m_depthStencilBuffer) {
        m_depthStencilBuffer->Release();
        m_depthStencilBuffer = nullptr;
    }

    if (m_renderTargetView) {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_deviceContext) {
        m_deviceContext->Release();
        m_deviceContext = nullptr;
    }

    if (m_device) {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_swapChain) {
        long _error = m_swapChain->Release();
        m_swapChain = nullptr;
    }

    return true;
}

GraphicsInterface * D3DClass::getInstance() {
    if (D3DClass::instance == nullptr) {
        D3DClass::instance = new D3DClass();
    }
    return D3DClass::instance;
}


ID3D11Device * D3DClass::GetDevice() const{
    return m_device;
}

ID3D11DeviceContext * D3DClass::GetDeviceContext() const{
    return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX & _matrix) {
    _matrix = m_projectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX & _matrix) {
    _matrix = m_worldMatrix;
}

void D3DClass::GetOrthoMatrix(XMMATRIX & _matrix) {
    _matrix = m_orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory) {
    strcpy_s(cardName, 128, m_videoCardDescription);
    memory = m_videoCardMemory;
}
