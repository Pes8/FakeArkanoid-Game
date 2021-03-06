#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "GraphicsManager.h"
#include "D3DClass.h"
#include "OpenGLClass.h"

#include <DirectXMath.h>

using namespace DirectX;




class WindowsGraphicsManager : public GraphicsManager {
public:
    virtual bool setup(GameConfig * config); //Get configuration from GameManager
    virtual bool initialization(); //Set all variables
    virtual bool run(); //Start the window
    virtual bool render(); // Do the rendering for the actual frame
    virtual bool shutdown();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

    static GraphicsManager * getInstance();
private:
    WindowsGraphicsManager();
    void initializeWindow();
    void showWindow();

    unsigned int m_iScreenWidth;
    unsigned int m_iScreenHeight;

    LPCSTR m_sApplicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    MSG m_oMsg;

    GraphicsInterface * m_o3DClass;

    //OpenGL stuff
    HDC m_hdc; // device context handle
    HGLRC m_hglrc; // OpenGL rendering context

    bool m_bVSyncEnabled;
    bool m_bFullscreen;
};

//static WindowsGraphicsManager* ApplicationHandle = 0; ##1##

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);