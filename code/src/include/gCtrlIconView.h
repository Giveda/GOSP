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

#ifndef GCTRLICONVIEW_H
#define GCTRLICONVIEW_H

#include "gMItem.h"
#include "gCtrlIconViewItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlIconViewMhL;

class GCtrlIconView : public GMContainerItem
{
    SET_CLASS_NAME(GCtrlIconView)
    G_DISABLE_ASSIGN(GCtrlIconView)
public:
    GCtrlIconView ( GCtrlForm* frm, GMItem* parent=0, const char* name=0 );
    virtual ~GCtrlIconView();
    void insertItem( GCtrlIconViewItem*, int index=-1);
    void removeItem( int index=-1 );
    void changeItem ( GCtrlIconViewItem *, int index=-1 );
    void setRowNums( unsigned int num );
    void setColumnNums( unsigned int num );
    int getMaxIconHeight();
    int getMaxIconWidth();
    GCtrlIconViewItem* item( int index=-1 );
    unsigned int count();
    void clear();
    void setFocusInImage( const GImage& );
    void setFocusOutImage( const GImage& );
    void setCurItemIndex( const int nIndex );
    void emitSelected( GCtrlIconViewItem* );

protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );
signals:
    GSignal<void ( const GCtrlIconViewItem* )> focusChangedTo_pI;

    GSignal<void ( int )> focusChangedTo_pi;

    GSignal<void ( const GCtrlIconViewItem* )> highlighted_pI;

    GSignal<void ( int )> highlighted_pi;

    GSignal<void ( const GCtrlIconViewItem* )> selected_pI;

    GSignal<void ( int )> selected_pi;

private slots:
    void slotFocusChangedTo();
    void slotLoseFocus();
    void slotGetFocus();
    void slotItemClicked( const GCtrlIconViewItem* pItem );
private:
    bool moveFocusLeft();
    bool moveFocusRight();
    bool moveFocusUp();
    bool moveFocusDown();
    void moveFocus( int toIndex );

private:
    GCtrlIconViewMhL *ikvLqH;
};

#endif


// a nice day ^_^
