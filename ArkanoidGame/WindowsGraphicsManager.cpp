#include "WindowsGraphicsManager.h"


WindowsGraphicsManager::WindowsGraphicsManager() {
    m_o3DClass = nullptr;
}


void Camera::render() {
    XMFLOAT3 up, position, lookAt;
    XMVECTOR upVector, positionVector, lookAtVector;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;


    // Setup the vector that points upwards.
    up.x = m_afUp.x;
    up.y = m_afUp.y;
    up.z = m_afUp.z;

    // Load it into a XMVECTOR structure.
    upVector = XMLoadFloat3(&up);

    // Setup the position of the camera in the world.
    position.x = m_afPosition.x;
    position.y = m_afPosition.y;
    position.z = m_afPosition.z;

    // Load it into a XMVECTOR structure.
    positionVector = XMLoadFloat3(&position);

    // Setup where the camera is looking by default.
    lookAt.x = m_afLookAt.x;
    lookAt.y = m_afLookAt.y;
    lookAt.z = m_afLookAt.z;

    // Load it into a XMVECTOR structure.
    lookAtVector = XMLoadFloat3(&lookAt);

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    pitch = m_afRotation.x * 0.0174532925f;
    yaw = m_afRotation.y * 0.0174532925f;
    roll = m_afRotation.z * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAtVector = XMVectorAdd(positionVector, lookAtVector);

    // Finally create the view matrix from the three updated vectors.
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


bool WindowsGraphicsManager::setup(GameConfig * config) {
    this->m_bFullscreen = config->fullscreen;
    this->m_iScreenWidth = config->screenWidth;
    this->m_iScreenHeight = config->screeHeight;
    this->m_bVSyncEnabled = config->vsyncEnabled;

    m_oCamera.m_fFar = config->cameraFar;
    m_oCamera.m_fNear = config->cameraNear;
    //Copy Position, Rotation, LookAt and Up togheter
    memcpy(&m_oCamera.m_afPosition, &config->cameraPosition, sizeof(config->cameraPosition) * 4); //Yes, the have the same order! :)

    //Force to use OpenGL also on windows
    if (config->forceOpenGL_Windows) {
        m_o3DClass = OpenGLClass::getInstance();
    } else {
        m_o3DClass = D3DClass::getInstance();
    }

    return m_o3DClass != nullptr;
}

bool WindowsGraphicsManager::initialization(){
    initializeWindow();
    ZeroMemory(&m_oMsg, sizeof(MSG));

    // Initialize the Direct3D object.
    bool result = m_o3DClass->initialize(m_iScreenWidth, m_iScreenHeight, m_bVSyncEnabled, m_bFullscreen, m_oCamera.m_fFar, m_oCamera.m_fNear, m_hwnd);
    if (!result) {
        MessageBox(m_hwnd, "Could not initialize Direct3D", "Error", MB_OK);
        return false;
    }

    Block b1;

    result = m_o3DClass->loadAsset(&b1);

    // Create the color shader object.
    m_ColorShader = new ColorShaderClass;
    if (!m_ColorShader) {
        return false;
    }

    // Initialize the color shader object.
    result = m_ColorShader->Initialize((reinterpret_cast<D3DClass*>(m_o3DClass))->GetDevice(), m_hwnd);
    if (!result) {
        MessageBox(m_hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
        return false;
    }

    return result;
}

bool WindowsGraphicsManager::run() {
    showWindow();

    return true;
}



bool WindowsGraphicsManager::render(){
    
    //windows messages handling ... I'm don't know what is it ... 
    if (PeekMessage(&m_oMsg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&m_oMsg);
        DispatchMessage(&m_oMsg);
    }

    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;


    // Clear the buffers to begin the scene.
    //m_o3DClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position.
    m_oCamera.render();

    // Get the world, view, and projection matrices from the camera and d3d objects.
    (reinterpret_cast<D3DClass *>(m_o3DClass))->GetWorldMatrix(worldMatrix);
    m_oCamera.GetViewMatrix(viewMatrix);
    (reinterpret_cast<D3DClass *>(m_o3DClass))->GetProjectionMatrix(projectionMatrix);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    //m_Model->Render(m_Direct3D->GetDeviceContext());

    // Render the model using the color shader.
    result = m_ColorShader->Render((reinterpret_cast<D3DClass *>(m_o3DClass))->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
    if (!result) {
        return false;
    }

    //now rendering
    return m_o3DClass->render();
}

bool WindowsGraphicsManager::shutdown() {

    bool _result = true;

    // Release the color shader object.
    if (m_ColorShader) {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = nullptr;
    }

    if (m_o3DClass) {
        _result &= m_o3DClass->shutdown();
        delete m_o3DClass;
        m_o3DClass = nullptr;
    }

    // Show the mouse cursor.
    ShowCursor(true);

    // Fix the display settings if leaving full screen mode.
    if (m_bFullscreen) {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove the window.
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // Remove the application instance.
    UnregisterClass(m_sApplicationName, m_hinstance);
    m_hinstance = NULL;

    return _result;
}

GraphicsManager * WindowsGraphicsManager::getInstance(){
    if (GraphicsManager::instance == nullptr) {
        GraphicsManager::instance = new WindowsGraphicsManager();
    }
    return GraphicsManager::instance;
}

void WindowsGraphicsManager::initializeWindow() {
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;
    int posX, posY;


    // Get the instance of this application.
    m_hinstance = GetModuleHandle(NULL);

    // Give the application a name.
    m_sApplicationName = "ArkanoidGame";

    // Setup the windows class with default settings.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_sApplicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&wc);

    
    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
    if (m_bFullscreen) {
        
        // Determine the resolution of the clients desktop screen.
        m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = m_iScreenWidth;
        dmScreenSettings.dmPelsHeight = m_iScreenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        posX = posY = 0;
    } else {

        // Place the window in the middle of the screen.
        posX = (GetSystemMetrics(SM_CXSCREEN) - m_iScreenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - m_iScreenHeight) / 2;
    }

    // Create the window with the screen settings and get the handle to it.
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_sApplicationName, m_sApplicationName,
                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
                            posX, posY, m_iScreenWidth, m_iScreenHeight, NULL, NULL, m_hinstance, NULL);

}

void WindowsGraphicsManager::showWindow() {
    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide the mouse cursor.
    ShowCursor(false);
}

LRESULT CALLBACK WindowsGraphicsManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    switch (umessage) {
        // Check if the window is being destroyed.
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        // Check if the window is being closed.
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        // All other messages pass to the message handler in the system class.
        default:
        {
            //TODO: check if is better to store another instance of the class in a gloab static var as in ##1## or do every time a reinterpret_cast. Reinterpret cast should not loss performance, right ?
            //return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
            return reinterpret_cast<WindowsGraphicsManager*>(WindowsGraphicsManager::getInstance())->MessageHandler(hwnd, umessage, wparam, lparam);
        }
    }
}