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

#ifndef GCTRLLISTBOX_H
#define GCTRLLISTBOX_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlListBoxItem : public GMCtrlItem
{
    SET_CLASS_NAME(GCtrlListBoxItem)
    G_DISABLE_ASSIGN(GCtrlListBoxItem)
public:
    GCtrlListBoxItem ( const GString& str, GCtrlForm* frm, GMItem* parent=0, const char* name="listBoxItem" );
    virtual ~GCtrlListBoxItem() {}
    GMScrollText* getTxt() {
        return &m_txt;
    }
    GString text() {
        return m_txt.text();
    }
    virtual void setGeometry( int, int, int, int );
protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );
protected:
    GMScrollText m_txt;
};

class GCtrlListBoxPixmap : public GCtrlListBoxItem
{
    SET_CLASS_NAME(GCtrlListBoxPixmap)
    G_DISABLE_ASSIGN(GCtrlListBoxPixmap)
public:
    GCtrlListBoxPixmap ( const GPixmap& pm, const GString& str, GCtrlForm* frm, GMItem* parent=0, const char* name="listBoxPixmap" );
    virtual void setGeometry( int, int, int, int );
protected:
    virtual void paintEvent ( GPainter& );
private:
    GMPixmap m_pix;
};

class GCtrlListBoxPrivate;

class GCtrlListBox : public GMContainerItem
{
    SET_CLASS_NAME(GCtrlListBox)
    G_DISABLE_ASSIGN(GCtrlListBox)
public:
    GCtrlListBox ( GCtrlForm* frm, GMItem* parent=0, const char* name=0 );
    virtual ~GCtrlListBox();
    void insertItem( GCtrlListBoxItem*, int index=-1);
    void removeItem( int index=-1 );
    void removeItem( GCtrlListBoxItem* );
    void changeItem ( GCtrlListBoxItem *, int index=-1 );
    void setRowNums( unsigned int num );
    void setColumnNums( unsigned int num );
    int getMaxIconHeight();
    int getMaxIconWidth();
    GCtrlListBoxItem* item( int index=-1 );
    int getCurItemIndex();
    unsigned int count();
    void clear();
    void setFocusInImage( const GImage& );
    void setFocusOutImage( const GImage& );
    void setCurItemIndex( const int nIndex );

    void emitSelected( GCtrlListBoxItem* );
    void setItemTextColor(GColor& c);
    void setItemTextFont( GFont f);
    GColor getItemTextColor();
    GFont getItemTextFont();
    void setItemScrollEnabled(bool enabled );
    void setPreviousItemKey( int nKey );
    void setNextItemKey( int nKey );
    void setLoopFocus( bool b );
    
protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );
signals:
    GSignal<void ( const GCtrlListBoxItem* )> focusChangedTo_pI;

    GSignal<void ( int )> focusChangedTo_pi;

    GSignal<void ( const GCtrlListBoxItem* )> highlighted_pI;

    GSignal<void ( int )> highlighted_pi;

    GSignal<void ( const GCtrlListBoxItem* )> selected_pI;

    GSignal<void ( int )> selected_pi;
private slots:
    void slotFocusChangedTo();
    void slotLoseFocus();
    void slotGetFocus();
private:
    bool moveFocusLeft();
    bool moveFocusRight();
    bool moveFocusUp();
    bool moveFocusDown();
    void moveFocus( int toIndex );

private:
    GCtrlListBoxPrivate *lbPriv;
};

#endif


// have a nice day ^_^
// have a nice day ^_^
