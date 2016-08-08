#pragma once

#include "Types.h"
#include "GameObject.h"
#include <vector>

class Scene {
public:
    static Scene * getInstance();
    void addObject(GameObject * _object);
    void removeObject(int _ID);
    const VertexInfo * getTotalVertices() const;
    const unsigned short * getTotalIndices() const;
    unsigned int getVerticesNumber() const;
    unsigned int getIndicesNumber() const;
    std::vector<GameObject *> getObjectList() const;
private:
    Scene();

    static Scene * instance;
    std::vector<GameObject *> m_oObjectsList;
    std::vector<VertexInfo> m_oTotalVerticesInfo;
    std::vector<unsigned short> m_siTotalIndices;
    unsigned int m_iVerticesNumber;
    unsigned int m_iIndicesNumber;
};
