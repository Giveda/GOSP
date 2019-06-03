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

#ifndef GCTRLICONVIEWITEM_H
#define GCTRLICONVIEWITEM_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlIconView;

class GCtrlIconViewItem : public GMCtrlItem
{
    SET_CLASS_NAME(GCtrlIconViewItem)
public:
    GCtrlIconViewItem( const GImage& img, const GString& str, GCtrlForm* frm, GCtrlIconView* pIconView, const char* name="iconViewItem" );
    virtual ~GCtrlIconViewItem();
    GMScrollText* getTxt() {
        return &m_txtTitle;
    }
    GMImage* getIcon() {
        return &m_imgIcon;
    }
    void setGeometry(int, int, int, int);
protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );
private:
    GMScrollText m_txtTitle;
    GMImage m_imgIcon;
};

#endif
