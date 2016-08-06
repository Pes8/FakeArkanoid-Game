#include "GenericAsset.h"

GenericAsset::~GenericAsset() {
    
    SAFE_DELETE(m_aoVertices);
    SAFE_DELETE(m_alIndices);

}

/*void GenericAsset::setPosition(Vector3 _newPos) {
    m_vPosition = _newPos;
    for (int i = 0; i < m_iVertexCount; ++i) {
        m_aoVertices[i].position += _newPos;
    }
}*/

/*Vector3 GenericAsset::getPosition() const {
    return m_vPosition;
}

Vector3 GenericAsset::getRotation() const {
    return m_vRotation;
}*/
