#pragma once

#include <string>
#include <map>
#include "Types.h"
#include "GameObject.h"
#include "Character.h"
#include "Block.h"
#include "Ball.h"
#include "Wall.h"

#define BLOCK_INDEX 0
#define WALLS_INDEX 20
#define PLAYER_INDEX 30
#define BALL_INDEX 40

class AssetsManager {
private:
    
    static AssetsManager * instance;
    std::map<int, Mesh*> m_oMeshMap;
    std::map<int, Texture*> m_oTexMap;

    Mesh * createAABBMesh(float _sizeX = 0.5f, float _sizeY = 0.25f, float _sizeZ = 0.5f, float startTexU = 0.0f, float endTexU = 0.25f, float startTexV = 0.0f, float endTexV = 0.25f, float texOffsetU = 0.0f, float texOffsetV = 0.0f);
    Mesh * importMeshFromObj(std::string _filename);
public:
    static AssetsManager * getInstance();
    void preloadAllAssets();
    AssetsManager() {};
    Character * createPlayer();
    Ball * createBall();
    Block * createBlock(int type = 0);
    Wall * createVerticalWall();
    Wall * createHorizontalWall();
    ~AssetsManager();
};