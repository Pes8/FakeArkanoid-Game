#pragma once

#include <stdint.h>
#include <functional>
#include <vector>
#include "stb_image.h"

/* UTILITIES */
#define SAFE_DELETE(x) if(x) { delete x; x = nullptr; }
#define CLEAR_KEY_POINTER_MAP(x) for(auto it = x.begin(); it != x.end(); ++it){ delete it->second; } x.clear();
#define CLEAR_POINTER_CONTAINER(x) for(auto it = x.begin(); it != x.end(); ++it){ delete *it; } x.clear();

/* CONSTANTS */
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
#define CAMERA_ORTO_WIDTH 20.5
#define CAMERA_ORTO_HEIGHT 20.0

#define PLAYER_VELOCITY 0.25f // Horizontal movement
#define PLAYER_START_POS { 0, - (CAMERA_ORTO_HEIGHT/2 - 1.5f), 0.0f }
#define PLAYER_LIVES 1
#define PLAYER_SIZE_X 1.0f
#define PLAYER_SIZE_Y 0.2f
#define PLAYER_SIZE_Z 0.5f
#define BALL_VELOCITY -0.2f // Vertical movement (falling down initially)
#define BALL_SCALE 0.15f
#define BLOCK_SIZE_X 0.5f
#define BLOCK_SIZE_Y 0.25f
#define BLOCK_SIZE_Z 0.5f
#define WALL_THICKNESS_X 0.25f
#define WALL_THICKNESS_Y 0.25f
#define WALL_THICKNESS_Z 0.5f
#define GRID_BLOCK_SIZE_X BLOCK_SIZE_X * 2
#define GRID_BLOCK_SIZE_Y BLOCK_SIZE_Y * 2
#define GRID_BLOCK_START_X - (CAMERA_ORTO_WIDTH/2 - WALL_THICKNESS_X - BLOCK_SIZE_X)
#define GRID_BLOCK_START_Y CAMERA_ORTO_HEIGHT/2 - WALL_THICKNESS_Y - BLOCK_SIZE_Y


#define PI 3.141592f
#define PI_DIV_2 1.570796f
#define PI_DIV_3 1.047197f
#define PI_DIV_4 0.785398f
#define PI_DIV_6 0.523598f
#define PI_DIV_12 0.261799f

#define FPS 120 // Frame Per Seconds
#define uSPF (1000000 / FPS) // Micro-Seconds Per Frame

#define LOCAL_PATH "./Data/"
#define TEXTURE_PATH LOCAL_PATH "Textures/"
#define MESH_PATH LOCAL_PATH "Meshes/"
#define LEVEL_PATH LOCAL_PATH "Levels/"

/*CUSTOM TYPES */
typedef uint8_t ButtonsStatus;
typedef std::function<void()> callback;

// struct ... one day I can take the configurations from a file...
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

    Vector3& operator*=(const Vector3 & _b) {
        this->x *= _b.x;
        this->y *= _b.y;
        this->z *= _b.z;
        return *this;
    }

    Vector3& operator*=(const float & _scalar) {
        this->x *= _scalar;
        this->y *= _scalar;
        this->z *= _scalar;
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

struct Mesh {
    VertexInfo * m_aoVertices = nullptr;
    unsigned int m_iVertexCount = 0;
    unsigned int m_iIndexCount = 0;
    unsigned short * m_alIndices = nullptr;
    ~Mesh() {
        delete[] m_aoVertices;
        delete[] m_alIndices;
    };
};

struct Texture {
    unsigned char * data = nullptr;
    int width = 0;
    int height = 0;
    int channels = 4;
    Vector4 defaultColor = { 0.6f, 0.3f, 0.2f, 1.0f };
    ~Texture(){
        stbi_image_free(data);
    }
};

struct Collider {
    Vector3 sizes;
    enum { SPHERE, AABB } type;
    float radius;
    bool active = false;
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

struct ID {
    static int _ID;
    int _MyID;
    ID() {
        ++ID::_ID;
        _MyID = ID::_ID;
    }
};

struct Event {
    std::vector<callback> subscriptions;
    unsigned int subscribe(callback cb) {
        subscriptions.push_back(cb);
        return subscriptions.size() -1;
    }
    void unsubscribe(unsigned int pos) {
        if(subscriptions.size() > pos)
            subscriptions.erase(subscriptions.begin() + pos);
    }
    void fire() { for (callback& cb : subscriptions) cb(); }
};