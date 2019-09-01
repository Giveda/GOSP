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

#ifndef GCTRLLINEEDIT_H
#define GCTRLLINEEDIT_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlLineEditMhL;

class GCtrlLineEdit : public GMCtrlItem
{
    SET_CLASS_NAME(GCtrlLineEdit)
    G_DISABLE_ASSIGN(GCtrlLineEdit)
public:
    enum	EchoMode { Normal, Password };
    GCtrlLineEdit ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    virtual ~GCtrlLineEdit();
    void setEchoMode( EchoMode mode );
    EchoMode 	echoMode() const;
    void setTextGeometry (int x, int y, int w, int h );

    void setFont (const GFont& font );

    void setColor (const GColor& color );

    void setText( const GString& str );

    GString text();

    void clear();

    void setBackspaceKey( int nKey );
protected:
    virtual bool fwKeyPressEvent( GKeyEvent * );
    virtual void paintEvent ( GPainter& );
private:
    void initAttributes ();

private slots:
    void slotTimeOut();
    void slotGetFocus();
    void slotLoseFocus();

private:
    GCtrlLineEditMhL *leLqH;
};

#endif


// have a nice day ^_^
