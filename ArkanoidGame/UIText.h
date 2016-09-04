#pragma once

#include "UIObject.h"
#include <string>

enum TextType : char {
    SMALL,
    MEDIUM,
    LARGE
};

struct UIText : public UIObject {
    
    UIText(wchar_t * _text, int _size, Vector2 _pos = { 0.0f, 0.0f }, TextType _type = MEDIUM);
    UIText(char * _text, int _size, Vector2 _pos = { 0.0f, 0.0f }, TextType _type = MEDIUM);
    UIText(const UIText&) = delete;
    ~UIText();
    void setText(char * _t);
    
    wchar_t * m_pText;
    int m_iSize;
    TextType m_eType;
};