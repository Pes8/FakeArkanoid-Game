#pragma once


#include "GraphicsManager.h"
#include <Windows.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
//#include "d3dx11.h"

class WindowsGraphicsManager : public GraphicsManager {
public:
    virtual void setup(GameConfig * config); //Get configuration from GameManager
    virtual void initialization(); //Set all variables
    virtual void run(); //Start the window
    virtual void render(); // Do the rendering for the actual frame
    virtual void shutdown();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

    static GraphicsManager * getInstance();
private:

    void initializeWindow();
    void showWindow();

    unsigned int m_iScreenWidth;
    unsigned int m_iScreenHeight;
    bool m_bFullscreen;

    LPCSTR m_sApplicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    MSG m_oMsg;
};

static WindowsGraphicsManager* ApplicationHandle = 0;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);