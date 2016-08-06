#pragma once

#include "Types.h"


class GraphicsManager {
public:
    virtual bool setup(GameConfig * config) = 0; //Get configuration from GameManager
    virtual bool initialization() = 0; //Set all variables
    virtual bool run() = 0; //Start the window
    virtual bool render() = 0; // Do the rendering for the actual frame
    virtual bool shutdown() = 0; // Close all

    virtual void CameraGoUp() = 0; //TODO remove

    static GraphicsManager * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    static GraphicsManager * instance;
    Camera m_oCamera;
};