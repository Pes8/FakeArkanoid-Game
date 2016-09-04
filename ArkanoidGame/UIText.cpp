#include "UIText.h"

UIText::UIText(wchar_t * _text, int _size, Vector2 _pos, TextType _type) {

    m_vStartRectPosition = _pos;
    m_vEndRectPosition.u = _pos.u + 12 * _size + _type;
    m_vEndRectPosition.v = _pos.v + 24 * (_type + 1);

    m_eType = _type;
    m_pText = new wchar_t[_size];
    m_iSize = _size;
    memcpy(m_pText, _text, _size * sizeof(wchar_t));
}

UIText::UIText(char * _text, int _size, Vector2 _pos, TextType _type) {
    m_iSize = _size;
    m_pText = new wchar_t[m_iSize];
    mbstowcs(m_pText, _text, m_iSize);

    m_vStartRectPosition = _pos;
    m_vEndRectPosition.u = _pos.u + 12 * _size + _type;
    m_vEndRectPosition.v = _pos.v + 24 * (_type + 1);
    m_eType = _type;
    
}

UIText::~UIText() {
    delete[] m_pText;
}

void UIText::setText(char * _t) {
    m_iSize = strlen(_t);
    delete[] m_pText;
    m_pText = new wchar_t[m_iSize];
    mbstowcs(m_pText, _t, m_iSize);
    m_vEndRectPosition.u = m_vStartRectPosition.u + 12 * m_iSize;
}
