#include "Scene.h"

Scene * Scene::instance = nullptr;

Scene * Scene::getInstance() {
    if (Scene::instance == nullptr) {
        Scene::instance = new Scene();
    }
    return Scene::instance;
}

void Scene::addObject(GenericAsset * _object) {
    m_oObjectsList.push_back(_object);
    
    unsigned short _oldVertNumber = m_iVerticesNumber;

    m_iVerticesNumber += _object->m_iVertexCount;
    m_iIndicesNumber += _object->m_iIndexCount;
    
    for (unsigned int i = 0; i < _object->m_iVertexCount; ++i) {
        m_oTotalVerticesInfo.push_back(_object->m_aoVertices[i]);
    }

    for (unsigned int i = 0; i < _object->m_iIndexCount; ++i) {
        m_siTotalIndices.push_back(_oldVertNumber + _object->m_alIndices[i]);
    }

}

void Scene::removeObject(int _ID) {}

const VertexInfo * Scene::getTotalVertices() const {
    return &(m_oTotalVerticesInfo[0]);
}

const unsigned short * Scene::getTotalIndices() const {
    return &(m_siTotalIndices[0]);
}

unsigned int Scene::getVerticesNumber() const {
    return m_iVerticesNumber;
}

unsigned int Scene::getIndicesNumber() const {
    return m_iIndicesNumber;
}

std::vector<GenericAsset*> Scene::getObjectList() const {
    return m_oObjectsList;
}

Scene::Scene() {}
