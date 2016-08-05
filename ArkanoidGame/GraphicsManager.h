#pragma once

#include "Types.h"

//TODO REMOVE AND THINK SOMETHING LIKE A "Asset Manager"
#include "Block.h"

struct Camera {
    Vector3 m_afPosition;
    Vector3 m_afRotation;
    Vector3 m_afLookAt;
    Vector3 m_afUp;
    float m_fFar;
    float m_fNear;

    void render();
};

class GraphicsManager {
public:
    virtual bool setup(GameConfig * config) = 0; //Get configuration from GameManager
    virtual bool initialization() = 0; //Set all variables
    virtual bool run() = 0; //Start the window
    virtual bool render() = 0; // Do the rendering for the actual frame
    virtual bool shutdown() = 0; // Close all

    static GraphicsManager * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    static GraphicsManager * instance;
    Camera m_oCamera;
};