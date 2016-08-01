#pragma once

#include <stdint.h>
#include <functional>
#include <vector>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_FULLSCREEN false
#define DEFAULT_STARTLIFES 3
#define DEFAULT_VSYNC_ENABLED true
#define DEFAULT_SCREEN_DEPTH 1000.0f
#define DEFAULT_SCREEN_NEAR 0.1f
#define DEFAULT_OPENGL_WINDOWS false

#define FPS 120 // Frame Per Seconds
#define uSPF (1000000 / FPS) // Micro-Seconds Per Frame

typedef void(*FP)();
typedef std::function<void()> callback;

// struct so one day I can take the configurations from a file...
struct GameConfig {
    unsigned int screenWidth = DEFAULT_WIDTH;
    unsigned int screeHeight = DEFAULT_HEIGHT;
    unsigned int startLife = DEFAULT_STARTLIFES;
    float screenDepth = DEFAULT_SCREEN_DEPTH;
    float screenNear = DEFAULT_SCREEN_NEAR;
    bool fullscreen = DEFAULT_FULLSCREEN;
    bool vsyncEnabled = DEFAULT_VSYNC_ENABLED;
    bool forceOpenGL_Windows = DEFAULT_OPENGL_WINDOWS;
};


enum Button {
    QUIT_BTN,
    PAUSE_BTN,
    ENTER_BTN,
    LEFT_BTN,
    RIGHT_BTN,
    UP_BTN,
    DOWN_BTN,
    NOTHING
};


struct Event {
    std::vector<callback> subscriptions;
    unsigned int subscribe(callback cb) {
        subscriptions.push_back(cb);
        return subscriptions.size();
    }
    void unsubcribe(unsigned int pos) {
        subscriptions.erase(subscriptions.begin() + pos);
    }
    void fire() { for (callback& cb : subscriptions) cb(); }
};

typedef uint8_t ButtonsStatus;