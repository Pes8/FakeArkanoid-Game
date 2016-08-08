#pragma once

#include <stdint.h>
#include <functional>
#include <vector>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_FULLSCREEN false
#define DEFAULT_STARTLIFES 3
#define DEFAULT_VSYNC_ENABLED true
#define DEFAULT_CAMERA_FAR 1000.0f
#define DEFAULT_CAMERA_NEAR 0.1f
#define DEFAULT_OPENGL_WINDOWS false
#define DEFAULT_CAMERA_POSITION {0.0f, 0.0f, -5.0f}
#define DEFAULT_CAMERA_ROTATION {0.0f, 0.0f, 0.0f}
#define DEFAULT_CAMERA_LOOKAT {0.0f, 0.0f, 1.0f}
#define DEFAULT_CAMERA_UP {0.0f, 1.0f, 0.0f}
#define CAMERA_ORTO_WIDTH 20.0
#define CAMERA_ORTO_HEIGHT 20.0


/* UTILITIES */
#define SAFE_DELETE(x) if(x) { delete x; x = nullptr; }

#define FPS 120 // Frame Per Seconds
#define uSPF (1000000 / FPS) // Micro-Seconds Per Frame

typedef void(*FP)();
typedef std::function<void()> callback;

// struct so one day I can take the configurations from a file...
struct GameConfig {
    unsigned int screenWidth = DEFAULT_WIDTH;
    unsigned int screeHeight = DEFAULT_HEIGHT;
    unsigned int startLife = DEFAULT_STARTLIFES;
    float cameraFar = DEFAULT_CAMERA_FAR;
    float cameraNear = DEFAULT_CAMERA_NEAR;
    float cameraPosition[3] = DEFAULT_CAMERA_POSITION;
    float cameraRotation[3] = DEFAULT_CAMERA_ROTATION;
    float cameraLookAt[3] = DEFAULT_CAMERA_LOOKAT;
    float cameraUp[3] = DEFAULT_CAMERA_UP;
    bool fullscreen = DEFAULT_FULLSCREEN;
    bool vsyncEnabled = DEFAULT_VSYNC_ENABLED;
    bool forceOpenGL_Windows = DEFAULT_OPENGL_WINDOWS;
};


struct Vector2{
    float u;
    float v;

    Vector2& operator+=(const Vector2 & _b) {
        this->u += _b.u;
        this->v += _b.v;
        return *this;
    }
};


struct Vector3{
    float x;
    float y;
    float z;

    Vector3& operator+=(const Vector3 & _b) {
        this->x += _b.x;
        this->y += _b.y;
        this->z += _b.z;
        return *this;
    }
};

struct Vector4{
    float x;
    float y;
    float z;
    float w;
};

struct VertexInfo {
    Vector3 position;
    Vector4 color;
    Vector2 uv;
};

struct Camera {
    Vector3 m_afPosition;
    Vector3 m_afRotation;
    Vector3 m_afLookAt;
    Vector3 m_afUp;
    float m_fFar;
    float m_fNear;
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