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

#include "gMItem.h"
#include "gCtrlView.h"
#include <gFontMetrics.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GMItemMhL
{
public:
    GMItemMhL(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) 
    {}
    
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iLqH( new GMItemMhL )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iLqH->m_pForm = form;
    setVisible ( true );
    iLqH->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iLqH;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iLqH->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iLqH->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iLqH->m_rect.width();
}
int GMItem::z() const
{
    return iLqH->m_nZ;
}
int GMItem::y() const
{
    return iLqH->m_rect.y();
}
int GMItem::x() const
{
    return iLqH->m_rect.x();
}
int GMItem::height() const
{
    return iLqH->m_rect.height();
}
int GMItem::right() const
{
    return iLqH->m_rect.right();
}
int GMItem::bottom() const
{
    return iLqH->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iLqH->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iLqH->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iLqH->m_pForm;
}
bool GMItem::isVisible()
{
    return iLqH->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iLqH->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iLqH->m_bgColor = c;
}
bool GMItem::isComplex()
{
    return true;
}
void GMItem::paintEvent ( GPainter& ) {}

void GMItem::show()
{
    setVisible ( true );
    update();
}

void GMItem::hide()
{
    setVisible ( false );
    update();
}

void GMItem::draw ( GPainter& p )
{
    if ( !iLqH->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iLqH->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iLqH->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iLqH->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iLqH->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iLqH->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iLqH->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iLqH->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iLqH->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iLqH->m_rect.moveBy ( x, y );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x, y );
    }
#endif
}

void GMItem::update()
{
    GRect r = rect();
    GMItem* pParent = ( GMItem* ) parent();
    while ( pParent )
    {
        r.moveBy ( pParent->x(), pParent->y() );
        pParent = ( GMItem* ) pParent->parent();
    }
    form()->update ( r );
}

class GMCtrlItemMhL
{
public:
    GMCtrlItemMhL() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciLqH( new GMCtrlItemMhL )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciLqH->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciLqH;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciLqH->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciLqH->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciLqH->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciLqH->m_nTabIndex;
}
bool GMCtrlItem::keyPressEvent ( GKeyEvent* )
{
    return false;
}

void GMCtrlItem::setFocus()
{
    if ( !isVisible() )
    {
        return;
    }
    else if ( !isFocusEnabled() )
    {
        return;
    }

    GMContainerItem* pContainer = ( GMContainerItem* ) parent();
    if ( pContainer )
    {
        pContainer->setFocusToItem ( this );
    }
    else
    {
        form()->setFocusToItem ( this );
    }
}

bool GMCtrlItem::hasFocus()
{
    return ciLqH->m_bIsHasFocus;
}

bool GMCtrlItem::fwKeyPress ( GKeyEvent *e )
{
    bool bRet = fwKeyPressEvent ( e );
    if ( true == bRet )
    {
        return true;
    }

    bRet = keyPressEvent ( e );

    return bRet;
}

void GMCtrlItem::emitLoseFocus()
{
    ciLqH->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciLqH->m_bIsHasFocus = true;
    getFocus.emit();
}

void GMCtrlItem::hide()
{
    if ( hasFocus() )
    {
        form()->setFocusToItem ( NULL );
    }

    GMItem::hide();
}

GMContainerItem::GMContainerItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMCtrlItem ( form, parent, name ), mpFocus ( NULL )
{
}

void GMContainerItem::setFocusToItem ( GMCtrlItem* pItem )
{
    if ( mpFocus )
    {
        if ( pItem != mpFocus )
        {
            mpFocus->emitLoseFocus();
            mpFocus->update();
        }
        else
        {
            return ;
        }
    }

    mpFocus = pItem;

    if ( mpFocus )
    {
        mpFocus->emitGetFocus();
        mpFocus->update();
    }
}

class GMPixmapMhL
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixLqH( new GMPixmapMhL )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixLqH( new GMPixmapMhL )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixLqH( new GMPixmapMhL )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixLqH;
}

void GMPixmap::load ( const GString& strPath )
{
    pixLqH->m_pixmap.load ( strPath );
    setSize ( pixLqH->m_pixmap.width(), pixLqH->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixLqH->m_pixmap = pm;
    setSize ( pixLqH->m_pixmap.width(), pixLqH->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixLqH->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixLqH->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixLqH->m_pixmap.width() +1 )
    {
        nX += ( width()-pixLqH->m_pixmap.width() ) /2;
    }
    if ( height() > pixLqH->m_pixmap.height() +1 )
    {
        nY += ( height()-pixLqH->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixLqH->m_pixmap );
}


class GMImageMhL
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgLqH( new GMImageMhL )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgLqH( new GMImageMhL )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgLqH( new GMImageMhL )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgLqH;
}

void GMImage::load ( const GString& strPath )
{
    imgLqH->m_imageOrg.load ( strPath );
    imgLqH->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgLqH->m_imageOrg.width(), imgLqH->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgLqH->m_imageOrg = img;
    imgLqH->m_imageScaled = img;
    GMItem::setSize ( imgLqH->m_imageOrg.width(), imgLqH->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if ( width() > imgLqH->m_imageScaled.width()
            || height() > imgLqH->m_imageScaled.height() )
    {
        imgLqH->m_imageScaled = imgLqH->m_imageOrg.smoothScale ( width(), height() );
    }

    int nX = x(), nY = y();
    int off = width()-imgLqH->m_imageScaled.width();
    if ( 1 < off )
    {
        nX += off /2;
    }
    off =  height()-imgLqH->m_imageScaled.height();
    if ( 1 < off )
    {
        nY += off /2;
    }
    p.drawImage ( nX, nY, imgLqH->m_imageScaled, 0, 0, width(), height() );
}

class GMTextMhL
{
public:
    GMTextMhL() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtLqH( new GMTextMhL )
{
    txtLqH->m_font = GFont ( "Sans", 20 );
    txtLqH->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtLqH->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtLqH( new GMTextMhL )
{
    txtLqH->m_str = str;
    txtLqH->m_font = GFont ( "Sans", 20 );
    txtLqH->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtLqH->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtLqH->m_font );
    txtLqH->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtLqH->m_str );
    setSize ( txtLqH->m_singleLineSize.width(), txtLqH->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtLqH;
}

void GMText::setColor ( const GColor& color )
{
    txtLqH->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtLqH->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtLqH->m_str;
}

GSize GMText::getSingleLineSize()
{
    return txtLqH->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtLqH->m_font );
    p.setPen ( txtLqH->m_color );
    p.drawText ( x(), y(), width(), height(), txtLqH->m_nFlags, txtLqH->m_str );
}

void GMText::setText ( const GString& str )
{
    txtLqH->m_str = str;
    GFontMetrics fontMetrics ( txtLqH->m_font );
    txtLqH->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtLqH->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtLqH->m_font = font;
    GFontMetrics fontMetrics ( txtLqH->m_font );
    txtLqH->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtLqH->m_str );
}

class GMScrollTextMhL
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stLqH( new GMScrollTextMhL )
{
    stLqH->m_pTimer = new GTimer ( this );
    connect ( stLqH->m_pTimer, stLqH->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stLqH->m_bIsResetX = true;
    stLqH->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stLqH( new GMScrollTextMhL )
{
    stLqH->m_pTimer = new GTimer ( this );
    connect ( stLqH->m_pTimer, stLqH->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stLqH->m_bIsResetX = true;
    stLqH->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stLqH;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stLqH->m_pTimer->start ( msec );
        stLqH->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stLqH->m_pTimer->stop();
        stLqH->m_bIsResetX = true;
        update();
        stLqH->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stLqH->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtLqH->m_singleLineSize.width() >width() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GMScrollText::slotUpdate()
{
    if ( !isVisible()
            || !form()->isVisible() 
            || stLqH->m_bShouldSroll == false )
    {
        return ;
    }
    
    update();
}


// have a nice day ^_^
// have a nice day ^_^
