#include "WindowsGraphicsManager.h"

void WindowsGraphicsManager::setup(GameConfig * config) {
    this->m_bFullscreen = config->fullscreen;
    this->m_iScreenWidth = config->screenWidth;
    this->m_iScreenHeight = config->screeHeight;
}

void WindowsGraphicsManager::initialization(){
    initializeWindow();
    ZeroMemory(&m_oMsg, sizeof(MSG));
}

void WindowsGraphicsManager::run() {
    showWindow();
}



void WindowsGraphicsManager::render(){
    
    if (PeekMessage(&m_oMsg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&m_oMsg);
        DispatchMessage(&m_oMsg);
    }

    //now rendering
}

void WindowsGraphicsManager::shutdown() {

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
    switch (umsg) {
        // Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN:
        {
            // If a key is pressed send it to the input object so it can record that state.
            int x = 10;
            return 0;
        }

        // Check if a key has been released on the keyboard.
        case WM_KEYUP:
        {
            // If a key is released then send it to the input object so it can unset the state for that key.
            int x = 10;
            return 0;
        }

        // Any other messages send to the default message handler as our application won't make use of them.
        default:
        {
            return DefWindowProc(hwnd, umsg, wparam, lparam);
        }
    }
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
            return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
        }
    }
}