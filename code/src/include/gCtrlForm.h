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

#ifndef GCTRLFORM_H
#define GCTRLFORM_H

#include "gMItem.h"
#include <gPtrList.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlView;
class GMItem;
class GMCtrlItem;
class GCtrlDefaultAppStyle;
class GKeyEvent;
class GCtrlFormMhL;

class GCtrlForm : public GObject
{
    G_DISABLE_COPY(GCtrlForm, GObject)
public:
    enum enumFocusMode {Auto_Focus=0, Manual_Focus=1};
    GCtrlForm( GCtrlForm* parent=0, const char* name=0 );
    virtual ~GCtrlForm();
    void setX (int x);
    void setY (int y);
    void setZ (int z);
    void setWidth( int w );
    void setHeight( int h );
    int x() const;
    int y() const;
    int z() const;
    int width () const;
    int height () const;
    GRect rect() const;
    void setPosition( int x, int y);
    void setSize( int w, int h );
    void setGeometry ( int x, int y, int w, int h);
    void moveBy ( int x, int y );

    void setPaletteBackgroundColor ( const GColor &c );
    void loadBackgroundPixmap( const GString& strPic );
    void loadBackgroundPixmap( const GPixmap& px );
    bool fwKeyPress(GKeyEvent* e);
    void paintEvent( GPainter &p );
    void appendItem( GMItem* );
    void inSort( GMItem* );
    void setFocusToItem(GMCtrlItem* pItem );
    void changeFocus(GMCtrlItem* pFrom, GMCtrlItem* pTo );
    GMCtrlItem* getFocusItem();
    void setFocus();
    unsigned int getMaxTabIndex();
    GCtrlView* view();
    void update(  GRect r ) ;
    void update();
    bool isVisible ();
    void setVisible (bool b);
    virtual void show();
    virtual void hide();
    GCtrlDefaultAppStyle* getDefaultAppStyle();
    bool hasFocus();
    void setFocusMode(enumFocusMode mode);
    enumFocusMode getFocusMode();
    bool isFocusEnabled();
    void setFocusEnabled( bool b );
    void loseFocus();
    void getFocus();


protected:
    virtual bool keyPressEvent(GKeyEvent* ); 
private:
    void initFocus();
    bool fwKeyPressEvent(GKeyEvent*);
private:
    GCtrlFormMhL *frmLqH;
};

#endif


