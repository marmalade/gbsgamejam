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

#include "Label.h"

Label::Label() : Actor(), Font(0)
{
    Colour.r = 255;
    Colour.g = 255;
    Colour.b = 255;
    Colour.a = 255;
    AlignH = IW_GX_FONT_ALIGN_LEFT;
    AlignV = IW_GX_FONT_ALIGN_TOP;
}

void Label::Release()
{
    Actor::Release();
}

void Label::Update()
{
    Actor::Update();
}

void Label::Render()
{
    if (Font != 0 && Visible)
    {
        IwGxLightingOn();
        IwGxFontSetCol(Colour);
        IwGxFontSetFont(Font);
        IwGxFontSetScale(IW_FIXED(Scale), IW_FIXED(Scale));
        IwGxFontSetRect(CIwRect((int16)Position.x, (int16)Position.y, (int16)IwGxGetScreenWidth(), (int16)IwGxGetScreenHeight()));
        IwGxFontSetAlignmentHor(AlignH);
        IwGxFontSetAlignmentVer(AlignV);
        IwGxFontDrawText(Text.c_str());
    }
}

void Label::setFont(CIwGxFont* font)
{
    Font = font;
}

void Label::setText(const char *text)
{
    Text = text;
}

void Label::setColour(uint8 r, uint8 g, uint8 b, uint8 a)
{
    Colour.r = r;
    Colour.g = g;
    Colour.b = b;
    Colour.a = a;
}

void Label::setAligment(IwGxFontAlignHor h, IwGxFontAlignVer v)
{
    AlignH = h;
    AlignV = v;
}
