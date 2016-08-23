#include "AssetsManager.h"
#include "stb_image.h"
#include <fstream>
#include <sstream>

AssetsManager * AssetsManager::instance = nullptr;

Mesh * AssetsManager::createAABBMesh(float _sizeX, float _sizeY, float _sizeZ, float startTexU, float endTexU, float startTexV, float endTexV, float texOffsetU, float texOffsetV) {
    
    Mesh * _oMesh = new Mesh();

    _oMesh->m_iVertexCount = 8;
    _oMesh->m_iIndexCount = 36;

    _oMesh->m_aoVertices = new VertexInfo[_oMesh->m_iVertexCount];
    _oMesh->m_aoVertices[0] = { - _sizeX,   _sizeY, - _sizeZ,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, startTexV + texOffsetV };
    _oMesh->m_aoVertices[1] = {   _sizeX,   _sizeY, - _sizeZ,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, startTexV + texOffsetV };
    _oMesh->m_aoVertices[2] = {   _sizeX,   _sizeY,   _sizeZ,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[3] = { - _sizeX,   _sizeY,   _sizeZ,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[4] = { - _sizeX, - _sizeY, - _sizeZ,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[5] = {   _sizeX, - _sizeY, - _sizeZ,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[6] = {   _sizeX, - _sizeY,   _sizeZ,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[7] = { - _sizeX, - _sizeY,   _sizeZ,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, endTexV + texOffsetV };


    _oMesh->m_alIndices = new unsigned short[_oMesh->m_iIndexCount]{
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };

    return _oMesh;
}

Mesh * AssetsManager::importMeshFromObj(std::string _filename) {
    std::ifstream infile(_filename);
    if (!infile.is_open()) return false;
    std::string line;

    std::vector< Vector3 > tmpV;
    std::vector< Vector3 > tmpN;
    

    std::vector< VertexInfo > tmpVertex;
    std::vector< short> tmpIndex;

    Mesh * _mesh = new Mesh();

    while (std::getline(infile, line)) {

        std::istringstream iss(line);

        std::string code;
        iss >> code;
        if (code == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            tmpV.push_back({ x, z, y });
        } else if (code == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            tmpN.push_back({ x, z, y });
        } else if (code == "vt") {
            float x, y;
            iss >> x >> y;
            VertexInfo nv;
            nv.uv = { x, 1.0f - y }; // NB: flipping the y, different conventions about UV space
            nv.color = { 1.0f, 0.341f, 0.133f, 1.0f };
            tmpVertex.push_back(nv);

        } else if (code == "f") {
            std::string st_i, st_j, st_k;
            iss >> st_i >> st_j >> st_k;
            int i0, i1, i2;
            int j0, j1, j2;
            int k0, k1, k2;
            sscanf_s(st_i.c_str(), "%d/%d/%d", &i0, &i1, &i2);
            sscanf_s(st_j.c_str(), "%d/%d/%d", &j0, &j1, &j2);
            sscanf_s(st_k.c_str(), "%d/%d/%d", &k0, &k1, &k2);

            //  Obj indices start from 1 not 0!
            i0--; j0--; k0--;
            i1--; j1--; k1--;
            i2--; j2--; k2--;

            tmpIndex.push_back(i1);
            tmpIndex.push_back(j1);
            tmpIndex.push_back(k1);


            tmpVertex[i1].position = tmpV[i0];
            //tmpVertex[i1].norm = tmpN[i2];
            tmpVertex[j1].position = tmpV[j0];
            //tmpVertex[j1].norm = tmpN[j2];
            tmpVertex[k1].position = tmpV[k0];
            //tmpVertex[k1].norm = tmpN[k2];

        }

    }

    _mesh->m_iVertexCount = tmpVertex.size();
    _mesh->m_iIndexCount = tmpIndex.size();

    _mesh->m_aoVertices = new VertexInfo[_mesh->m_iVertexCount];
    _mesh->m_alIndices = new unsigned short[_mesh->m_iIndexCount];

    memcpy(_mesh->m_aoVertices, &tmpVertex[0], sizeof(VertexInfo) * _mesh->m_iVertexCount);
    memcpy(_mesh->m_alIndices, &tmpIndex[0], sizeof(short) * _mesh->m_iIndexCount);

    return _mesh;
}

AssetsManager * AssetsManager::getInstance() {
    if (AssetsManager::instance == nullptr) {
        AssetsManager::instance = new AssetsManager();
    }
    return AssetsManager::instance;
}

void AssetsManager::preloadAllAssets() {
    
    /** TEXTURES **/

    //Player Texture
    Texture * _oTexturePlayer = new Texture();
    _oTexturePlayer->data = stbi_load("./Data/Textures/Player-Texture.png", &_oTexturePlayer->width, &_oTexturePlayer->height, &_oTexturePlayer->channels, _oTexturePlayer->channels);
    m_oTexMap[PLAYER_INDEX] = _oTexturePlayer;

    //Ball Texture
    Texture * _oTextureBall = new Texture();
    _oTextureBall->data = stbi_load("./Data/Textures/Ball-Texture.jpg", &_oTextureBall->width, &_oTextureBall->height, &_oTextureBall->channels, _oTextureBall->channels);
    m_oTexMap[BALL_INDEX] = _oTextureBall;

    //Walls Texture
    Texture * _oTextureWall = new Texture();
    _oTextureWall->data = stbi_load("./Data/Textures/Walls-Texture.gif", &_oTextureWall->width, &_oTextureWall->height, &_oTextureWall->channels, _oTextureWall->channels);
    m_oTexMap[WALLS_INDEX] = _oTextureWall;

    //Blocks Texture
    Texture * _oTexturBlocks = new Texture();
    _oTexturBlocks->data = stbi_load("./Data/Textures/Blocks-Texture.png", &_oTexturBlocks->width, &_oTexturBlocks->height, &_oTexturBlocks->channels, _oTexturBlocks->channels);
    m_oTexMap[BLOCK_INDEX] = _oTexturBlocks;


    /** MESHES **/

    m_oMeshMap[BLOCK_INDEX + 0] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.0f);                    // Red - Normal
    m_oMeshMap[BLOCK_INDEX + 1] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.0f);                   // Yellow - Normal
    m_oMeshMap[BLOCK_INDEX + 2] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.0f);                    // Cyan - Normal
    m_oMeshMap[BLOCK_INDEX + 3] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.0f);                   // Purple - Normal
    m_oMeshMap[BLOCK_INDEX + 4] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.25f);                   // Orange - Normal
    m_oMeshMap[BLOCK_INDEX + 5] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.25f);                  // Green - Normal
    m_oMeshMap[BLOCK_INDEX + 6] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.25f);                   // Blue - Normal
    m_oMeshMap[BLOCK_INDEX + 7] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.25f);                  // White - Normal
    m_oMeshMap[BLOCK_INDEX + 8] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.5f);                    // Gold1 - Hard
    m_oMeshMap[BLOCK_INDEX + 9] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.75f);                   // Gold2 - Hard
    m_oMeshMap[BLOCK_INDEX + 10] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.5f);                  // Double Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 11] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.75f);                 // Double Gray2 - Hard
    m_oMeshMap[BLOCK_INDEX + 12] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.5f);                   // Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 13] = createAABBMesh(0.5f, 0.25f, 0.5f, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.75f);                  // Gray2 - Hard

    m_oMeshMap[WALLS_INDEX + 0] = createAABBMesh(0.25f, 10.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);                                 // Wall - vertical
    m_oMeshMap[WALLS_INDEX + 1] = createAABBMesh(10.25f, 0.25f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);                                // Wall - horizontal

    m_oMeshMap[PLAYER_INDEX] = createAABBMesh(1.0f, 0.20f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);                                     // Player

    m_oMeshMap[BALL_INDEX] = importMeshFromObj("./Data/Meshes/test-Sphere.obj"); // Ball
    //m_oMeshMap[BALL_INDEX] = createAABBMesh(0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 1.0f);                  // TMP - Ball

}

Character * AssetsManager::createPlayer() {
    Character * player = new Character();
    player->m_oMesh = m_oMeshMap[PLAYER_INDEX];
    player->m_oTexture = m_oTexMap[PLAYER_INDEX];
    player->hasTexture = player->m_oTexture && player->m_oTexture->data;
    player->m_vPosition = { 0, -8.5f, 0.0f };
    player->m_vRotation = { 0.0f, 0.0f, 0.0f };
    player->scale = 1.0f;

    player->collider.active = true;
    player->collider.sizes = { 1.0f, 0.2f, 0.5f };
    player->collider.type = Collider::AABB;

    return player;
}

Block * AssetsManager::createBall() {
    Block * ball = new Block();
    ball->m_oMesh = m_oMeshMap[BALL_INDEX];
    ball->m_oTexture = m_oTexMap[BALL_INDEX];
    ball->hasTexture = ball->m_oTexture && ball->m_oTexture->data;
    ball->m_vRotation = { 0.0f, 0.0f, 0.0f };
    ball->m_vPosition = { 0.0f, 0.0f, 0.0f };
    ball->scale = 0.25f;
    
    ball->collider.active = true;
    ball->collider.radius = ball->scale;
    ball->collider.type = Collider::SPHERE;
    
    ball->m_vVelocity = { 0, -0.15f, 0 };

    return ball;
}

Block * AssetsManager::createBlock(int type) {
    Block * _block = new Block();
    _block->m_oMesh = m_oMeshMap[BLOCK_INDEX + type];
    _block->m_oTexture = m_oTexMap[BLOCK_INDEX];
    _block->hasTexture = _block->m_oTexture && _block->m_oTexture->data;
    _block->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _block->scale = 1.0f;

    _block->collider.active = true;
    _block->collider.sizes = { 0.5f, 0.5f, 0.5f };
    _block->collider.type = Collider::AABB;

    return _block;
}

Block * AssetsManager::createVerticalWall() {
    Block * _wall = new Block();
    _wall->m_oMesh = m_oMeshMap[WALLS_INDEX + 0];
    _wall->m_oTexture = m_oTexMap[WALLS_INDEX];
    _wall->hasTexture = _wall->m_oTexture && _wall->m_oTexture->data;
    _wall->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _wall->scale = 1.0f;

    _wall->collider.active = true;
    _wall->collider.sizes = { 0.25f, 10.0f, 0.25f };
    _wall->collider.type = Collider::AABB;

    return _wall;
}

Block * AssetsManager::createHorizontalWall() {
    Block * _wall = new Block();
    _wall->m_oMesh = m_oMeshMap[WALLS_INDEX + 1];
    _wall->m_oTexture = m_oTexMap[WALLS_INDEX];
    _wall->hasTexture = _wall->m_oTexture && _wall->m_oTexture->data;
    _wall->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _wall->scale = 1.0f;

    _wall->collider.active = true;
    _wall->collider.sizes = { 10.0f, 0.25f, 0.25f };
    _wall->collider.type = Collider::AABB;

    return _wall;
}

AssetsManager::~AssetsManager() {
//?? chiamare la delete su tutti gli iggetti prima di fre un map.clear ??
}
