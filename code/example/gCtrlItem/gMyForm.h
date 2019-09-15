/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 *
 * This program is an open-source software; and it is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 * This program is not a free software; so you can not redistribute it(include
 * binary form and source code form) without my authorization. And if you
 * need use it for any commercial purpose, you should first get commercial
 * authorization from me, otherwise there will be your's legal&credit risks.
 *
 */

#ifndef GMYFORM_H
#define GMYFORM_H

#include <gCtrlForm.h>
#include <gCtrlItem.h>

class GMyForm : public GCtrlForm
{
public:
    GMyForm( GCtrlForm* parent=0, const char* name=0 );
    virtual ~GMyForm();

protected:
    virtual bool keyPressEvent(GKeyEvent* );

private:
    GCtrlText m_txtImg;
    GCtrlImage m_img;
    GCtrlText m_txtPixmap;
    GCtrlPixmap m_pix;
};

#endif

// have a nice day ^_^
// have a nice day ^_^
