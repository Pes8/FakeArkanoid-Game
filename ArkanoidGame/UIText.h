#pragma once

#include "UIObject.h"
#include <string>


struct UIText : public UIObject {
    enum type {
        SMALL,
        MEDIUM,
        LARGE
    } m_eType;

    int m_iSize;
    wchar_t * m_pText;
    UIText(wchar_t * _text, int _size, Vector2 _pos = {0.0f, 0.0f}, type _type = MEDIUM);
    UIText(char * _text, int _size, Vector2 _pos = { 0.0f, 0.0f }, type _type = MEDIUM);
    UIText(const UIText&) = delete;
    ~UIText();
    void setText(char * _t);
};