#pragma once

#include "Types.h"

class GraphicsManager {
public:
    virtual void setup(GameConfig * config) = 0; //Get configuration from GameManager
    virtual void initialization() = 0; //Set all variables
    virtual void run() = 0; //Start the window
    virtual void render() = 0; // Do the rendering for the actual frame
    virtual void shutdown() = 0; // Close all

    static GraphicsManager * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    static GraphicsManager * instance;
};