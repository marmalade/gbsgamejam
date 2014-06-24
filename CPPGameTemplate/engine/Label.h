/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
 
#if !defined(_LABEL_H)
#define _LABEL_H

#include <string>
#include "IwGxFont.h"	// Marmalade font API

#include "Actor.h"

/**
 * @class Label
 *
 * @brief A Label is sued to render text to the display using a true type font
 *
 * Note that rotation and z depth are not supported
 */
class Label: public Actor
{
protected:
    CIwGxFont*			Font;		// Font used to render text
    std::string			Text;		// Text to display
    CIwColour			Colour;		// Colour text
    IwGxFontAlignHor	AlignH;		// Horizontal aligment of text
    IwGxFontAlignVer	AlignV;		// vertical aligment of text
public:
    Label();

    void			Release();
    void            Update();
    void            Render();

    // Setters / getters
    void			setFont(CIwGxFont* font);
    void			setText(const char *text);
    void            setColour(uint8 r, uint8 g, uint8 b, uint8 a);
    void            setAligment(IwGxFontAlignHor h, IwGxFontAlignVer v);
};


#endif  // _LABEL_H
