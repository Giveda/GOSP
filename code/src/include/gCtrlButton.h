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

#ifndef GCTRLBUTTON_H
#define GCTRLBUTTON_H

#include <gConstDefine.h>
#include <gGlobal.h>
#include "gMItem.h"

class GMButtonMhL;

class GMButton : public GMCtrlItem
{
    SET_CLASS_NAME(GMButton)
    G_DISABLE_ASSIGN(GMButton)
public:
    GMButton( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GMButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    virtual ~GMButton();

    void setCaption( GString strCaption );
    void setBgPixmap(const GPixmap &pix); 
    void setFocusPixmap(const GPixmap &pix); 
protected:
    virtual void paintEvent ( GPainter& );
    virtual bool fwKeyPressEvent(GKeyEvent*);
signals:
    GSignal<void(void)> clicked;
private:
    GMButtonMhL *btnLqH;
};

class GCtrlButton : public GMButton
{
    G_DISABLE_ASSIGN(GCtrlButton)
public:
    GCtrlButton( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlButton( const GString& strCaption, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
};

#endif


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
