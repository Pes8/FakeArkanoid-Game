#pragma once

#include "Types.h"
#include "GameObject.h"
#include "UItext.h"

class GraphicsManager {
public:
    virtual bool setup(GameConfig * config) = 0; //Get configuration from GameManager
    virtual bool initialization() = 0; //Set all variables
    virtual bool run() = 0; //Start the window
    virtual bool render() = 0; // Do the rendering for the actual frame
    virtual bool shutdown() = 0; // Close all

    void setScene(const std::vector<GameObject*> * _scene);
    void setUI(const std::vector<UIText*>* _ui);

    static GraphicsManager * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    
    
    const std::vector<GameObject*> * scene;
    const std::vector<UIText*> * ui;
    static GraphicsManager * instance;
    Camera m_oCamera;
    GameConfig * g_pConfig;
};