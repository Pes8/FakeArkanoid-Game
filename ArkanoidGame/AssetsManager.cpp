#include "AssetsManager.h"
#include "ThirdPartyLib/stb_image.h"
#include <fstream>
#include <sstream>

AssetsManager * AssetsManager::instance = nullptr;

Mesh * AssetsManager::createQuad(float _sizeX, float _sizeY, float startTexU, float endTexU, float startTexV, float endTexV, float texOffsetU, float texOffsetV) {
    
    Mesh * _oMesh = new Mesh();
    _oMesh->m_iVertexCount = 4;
    _oMesh->m_iIndexCount = 6;

    _oMesh->m_aoVertices = new VertexInfo[_oMesh->m_iVertexCount];
    _oMesh->m_aoVertices[0] = { - _sizeX, - _sizeY, 0,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, endTexV + texOffsetV };
    _oMesh->m_aoVertices[1] = { - _sizeX,   _sizeY, 0,  0.0f,0.0f,0.0f,0.0f, startTexU + texOffsetU, startTexV + texOffsetV };
    _oMesh->m_aoVertices[2] = {   _sizeX,   _sizeY, 0,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, startTexV + texOffsetV };
    _oMesh->m_aoVertices[3] = {   _sizeX, - _sizeY, 0,  0.0f,0.0f,0.0f,0.0f, endTexU + texOffsetU, endTexV + texOffsetV };

    _oMesh->m_alIndices = new unsigned short[_oMesh->m_iIndexCount]{
        0,1,3,
        3,1,2
    };

    return _oMesh;
}

Mesh * AssetsManager::createCircle(float _r, int _resolution) {
    
    Mesh * _oMesh = new Mesh();
    _oMesh->m_iVertexCount = _resolution + 2;
    _oMesh->m_aoVertices = new VertexInfo[_oMesh->m_iVertexCount];

    _oMesh->m_iIndexCount = 3 * _resolution;
    _oMesh->m_alIndices = new unsigned short[_oMesh->m_iIndexCount];

    _oMesh->m_aoVertices[0] = { 0, 0, 0.0f , 0.0f,0.0f,0.0f,0.0f, 0.0f, 0.0f };

    for (int i = 0; i < _resolution+1; ++i) {
        float x = - _r * cos(PI*(i / (_resolution / 2.0f)));
        float y = _r * sin(PI*(i / (_resolution / 2.0f)));
        _oMesh->m_aoVertices[i+1] = { x, y, 0.0f , 0.0f,0.0f,0.0f,0.0f, (x + 1.0f) / 2, (y + 1.0f) / 2 };
    }

    for (int j=0,k=0; j < _resolution; ++j, k+=3) {
        _oMesh->m_alIndices[k] = 0;
        _oMesh->m_alIndices[k+1] = j+1;
        _oMesh->m_alIndices[k+2] = j+2;
    }

    return _oMesh;
}

Mesh * AssetsManager::create3DAABBMesh(float _sizeX, float _sizeY, float _sizeZ, float startTexU, float endTexU, float startTexV, float endTexV, float texOffsetU, float texOffsetV) {
    
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

        0,5,4, // Davanti
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
    if (!infile.is_open()) return nullptr;
    std::string line;

    std::vector< Vector3 > tmpV;
    

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
        } /*else if (code == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            tmpN.push_back({ x, z, y });
        } */else if (code == "vt") {
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
            tmpVertex[j1].position = tmpV[j0];
            tmpVertex[k1].position = tmpV[k0];


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
    _oTexturePlayer->data = stbi_load(TEXTURE_PATH "Player-Texture.png", &_oTexturePlayer->width, &_oTexturePlayer->height, &_oTexturePlayer->channels, _oTexturePlayer->channels);
    m_oTexMap[PLAYER_INDEX] = _oTexturePlayer;

    //Ball Texture
    Texture * _oTextureBall = new Texture();
    _oTextureBall->data = stbi_load(TEXTURE_PATH "Ball-Texture.jpg", &_oTextureBall->width, &_oTextureBall->height, &_oTextureBall->channels, _oTextureBall->channels);
    m_oTexMap[BALL_INDEX] = _oTextureBall;

    //Walls Texture
    Texture * _oTextureWallV = new Texture();
    _oTextureWallV->data = stbi_load(TEXTURE_PATH "Wall-Texture-V.jpg", &_oTextureWallV->width, &_oTextureWallV->height, &_oTextureWallV->channels, _oTextureWallV->channels);
    m_oTexMap[WALLS_INDEX + 0] = _oTextureWallV;

    Texture * _oTextureWallH = new Texture();
    _oTextureWallH->data = stbi_load(TEXTURE_PATH "Wall-Texture-H.jpg", &_oTextureWallH->width, &_oTextureWallH->height, &_oTextureWallH->channels, _oTextureWallH->channels);
    m_oTexMap[WALLS_INDEX + 1] = _oTextureWallH;

    //Blocks Texture
    Texture * _oTexturBlocks = new Texture();
    _oTexturBlocks->data = stbi_load(TEXTURE_PATH "Blocks-Texture.png", &_oTexturBlocks->width, &_oTexturBlocks->height, &_oTexturBlocks->channels, _oTexturBlocks->channels);
    m_oTexMap[BLOCK_INDEX + 0] = _oTexturBlocks;

    Texture * _oTexturBlocksC = new Texture();
    _oTexturBlocksC->data = stbi_load(TEXTURE_PATH "Cracked-Blocks-Texture.png", &_oTexturBlocksC->width, &_oTexturBlocksC->height, &_oTexturBlocksC->channels, _oTexturBlocksC->channels);
    m_oTexMap[BLOCK_INDEX + 1] = _oTexturBlocksC;


    /** MESHES **/
#if OBJECTS_3D
    
    m_oMeshMap[BLOCK_INDEX + 0] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.0f);                    // Red - Normal
    m_oMeshMap[BLOCK_INDEX + 1] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.0f);                   // Yellow - Normal
    m_oMeshMap[BLOCK_INDEX + 2] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.0f);                    // Cyan - Normal
    m_oMeshMap[BLOCK_INDEX + 3] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.0f);                   // Purple - Normal
    m_oMeshMap[BLOCK_INDEX + 4] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.25f);                   // Orange - Normal
    m_oMeshMap[BLOCK_INDEX + 5] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.25f);                  // Green - Normal
    m_oMeshMap[BLOCK_INDEX + 6] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.25f);                   // Blue - Normal
    m_oMeshMap[BLOCK_INDEX + 7] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.25f);                  // White - Normal
    m_oMeshMap[BLOCK_INDEX + 8] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.5f);                    // Gold1 - Hard
    m_oMeshMap[BLOCK_INDEX + 9] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.75f);                   // Gold2 - Hard
    m_oMeshMap[BLOCK_INDEX + 10] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.5f);                  // Double Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 11] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.75f);                 // Double Gray2 - Hard
    m_oMeshMap[BLOCK_INDEX + 12] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.5f);                   // Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 13] = create3DAABBMesh(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.75f);                  // Gray2 - Hard

    m_oMeshMap[WALLS_INDEX + 0] = create3DAABBMesh(WALL_THICKNESS_X, CAMERA_ORTO_HEIGHT/2, WALL_THICKNESS_Z, 0.0f, 1.0f, 0.0f, 20.0f);                 // Wall - vertical
    m_oMeshMap[WALLS_INDEX + 1] = create3DAABBMesh(CAMERA_ORTO_WIDTH /2, WALL_THICKNESS_Y, WALL_THICKNESS_Z, 0.0f, 20.0f, 0.0f, 1.0f);                 // Wall - horizontal

    m_oMeshMap[PLAYER_INDEX] = create3DAABBMesh(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f, 1.0f, 0.0f, 1.0f);                                     // Player

    m_oMeshMap[BALL_INDEX] = importMeshFromObj(MESH_PATH "test-Sphere.obj"); // Ball
#else
    m_oMeshMap[BLOCK_INDEX + 0] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.0f);                    // Red - Normal
    m_oMeshMap[BLOCK_INDEX + 1] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.0f);                   // Yellow - Normal
    m_oMeshMap[BLOCK_INDEX + 2] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.0f);                    // Cyan - Normal
    m_oMeshMap[BLOCK_INDEX + 3] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.0f);                   // Purple - Normal
    m_oMeshMap[BLOCK_INDEX + 4] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.25f);                   // Orange - Normal
    m_oMeshMap[BLOCK_INDEX + 5] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.25f);                  // Green - Normal
    m_oMeshMap[BLOCK_INDEX + 6] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.25f);                   // Blue - Normal
    m_oMeshMap[BLOCK_INDEX + 7] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.75f, 0.25f);                  // White - Normal
    m_oMeshMap[BLOCK_INDEX + 8] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.5f);                    // Gold1 - Hard
    m_oMeshMap[BLOCK_INDEX + 9] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.0f, 0.75f);                   // Gold2 - Hard
    m_oMeshMap[BLOCK_INDEX + 10] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.5f);                  // Double Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 11] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.75f);                 // Double Gray2 - Hard
    m_oMeshMap[BLOCK_INDEX + 12] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.5f);                   // Gray1 - Hard
    m_oMeshMap[BLOCK_INDEX + 13] = createQuad(BLOCK_SIZE_X, BLOCK_SIZE_Y, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.75f);                  // Gray2 - Hard

    m_oMeshMap[WALLS_INDEX + 0] = createQuad(WALL_THICKNESS_X, CAMERA_ORTO_HEIGHT / 2, 0.0f, 1.0f, 0.0f, 20.0f);                 // Wall - vertical
    m_oMeshMap[WALLS_INDEX + 1] = createQuad(CAMERA_ORTO_WIDTH / 2, WALL_THICKNESS_Y, 0.0f, 20.0f, 0.0f, 1.0f);                 // Wall - horizontal

    m_oMeshMap[PLAYER_INDEX] = createQuad(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f, 1.0f, 0.0f, 1.0f);                                     // Player

    m_oMeshMap[BALL_INDEX] = createCircle(1.0f, 64);
#endif

    

}

Character * AssetsManager::createPlayer() {
    Character * player = new Character();
    player->m_oMesh = m_oMeshMap[PLAYER_INDEX];
    player->m_oTexture = m_oTexMap[PLAYER_INDEX];

    player->m_vPosition = PLAYER_START_POS;
    player->m_vRotation = { 0.0f, 0.0f, 0.0f };
    player->scale = 1.0f;

    return player;
}

Ball * AssetsManager::createBall() {
    Ball * ball = new Ball();
    ball->m_oMesh = m_oMeshMap[BALL_INDEX];
    ball->m_oTexture = m_oTexMap[BALL_INDEX];

    ball->m_vRotation = { 0.0f, 0.0f, 0.0f };
    ball->m_vPosition = BALL_POSITION;
    ball->scale = BALL_SCALE;
    
    ball->collider->active = true;
    //ball->collider->radius = ball->scale;
    ball->collider->sizes = {ball->scale, ball->scale, ball->scale};
    ball->collider->type = Collider::SPHERE;
    
    ball->m_vVelocity = { 0, BALL_VELOCITY, 0 };

    return ball;
}

Block * AssetsManager::createBlock(int type) {
    Block * _block = new Block();
    _block->m_oMesh = m_oMeshMap[BLOCK_INDEX + type];
    _block->m_oTexture = m_oTexMap[BLOCK_INDEX];

    _block->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _block->scale = 1.0f;

    _block->collider->active = true;
    _block->collider->sizes = { BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z };
    _block->collider->type = Collider::AABB;

    _block->hitBeforeDestroy = type / 8 + 1;

    return _block;
}

Wall * AssetsManager::createVerticalWall() {
    Wall * _wall = new Wall();
    _wall->m_oMesh = m_oMeshMap[WALLS_INDEX + 0];
    _wall->m_oTexture = m_oTexMap[WALLS_INDEX + 0];

    _wall->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _wall->scale = 1.0f;

    _wall->collider->active = true;
    _wall->collider->sizes = { WALL_THICKNESS_X, CAMERA_ORTO_HEIGHT / 2, WALL_THICKNESS_Z };
    _wall->collider->type = Collider::AABB;

    return _wall;
}

Wall * AssetsManager::createHorizontalWall() {
    Wall * _wall = new Wall();
    _wall->m_oMesh = m_oMeshMap[WALLS_INDEX + 1];
    _wall->m_oTexture = m_oTexMap[WALLS_INDEX + 1];

    _wall->m_vRotation = { 0.0f, 0.0f, 0.0f };
    _wall->scale = 1.0f;

    _wall->collider->active = true;
    _wall->collider->sizes = { CAMERA_ORTO_WIDTH / 2, WALL_THICKNESS_Y, WALL_THICKNESS_Z };
    _wall->collider->type = Collider::AABB;

    return _wall;
}

Texture * AssetsManager::getTexture(int id) const{
    return m_oTexMap.at(id);
}

const std::map<int, Texture*>* const AssetsManager::getAllTextures() const {
    return &m_oTexMap;
}

const std::map<int, Mesh*>* const AssetsManager::getAllMeshes() const {
    return &m_oMeshMap;
}

AssetsManager::~AssetsManager() {

    CLEAR_KEY_POINTER_MAP(m_oMeshMap);
    CLEAR_KEY_POINTER_MAP(m_oTexMap);

}