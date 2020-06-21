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

class GMItemHmq
{
public:
    GMItemHmq(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) 
    {}
    
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iMqs( new GMItemHmq )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iMqs->m_pForm = form;
    setVisible ( true );
    iMqs->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iMqs;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iMqs->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iMqs->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iMqs->m_rect.width();
}
int GMItem::z() const
{
    return iMqs->m_nZ;
}
int GMItem::y() const
{
    return iMqs->m_rect.y();
}
int GMItem::x() const
{
    return iMqs->m_rect.x();
}
int GMItem::height() const
{
    return iMqs->m_rect.height();
}
int GMItem::right() const
{
    return iMqs->m_rect.right();
}
int GMItem::bottom() const
{
    return iMqs->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iMqs->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iMqs->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iMqs->m_pForm;
}
bool GMItem::isVisible()
{
    return iMqs->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iMqs->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iMqs->m_bgColor = c;
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
    if ( !iMqs->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iMqs->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iMqs->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iMqs->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iMqs->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iMqs->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iMqs->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iMqs->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iMqs->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iMqs->m_rect.moveBy ( x, y );
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

class GMCtrlItemHmq
{
public:
    GMCtrlItemHmq() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciMqs( new GMCtrlItemHmq )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciMqs->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciMqs;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciMqs->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciMqs->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciMqs->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciMqs->m_nTabIndex;
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
    return ciMqs->m_bIsHasFocus;
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
    ciMqs->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciMqs->m_bIsHasFocus = true;
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

class GMPixmapHmq
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixMqs( new GMPixmapHmq )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixMqs( new GMPixmapHmq )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixMqs( new GMPixmapHmq )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixMqs;
}

void GMPixmap::load ( const GString& strPath )
{
    pixMqs->m_pixmap.load ( strPath );
    setSize ( pixMqs->m_pixmap.width(), pixMqs->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixMqs->m_pixmap = pm;
    setSize ( pixMqs->m_pixmap.width(), pixMqs->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixMqs->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixMqs->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixMqs->m_pixmap.width() +1 )
    {
        nX += ( width()-pixMqs->m_pixmap.width() ) /2;
    }
    if ( height() > pixMqs->m_pixmap.height() +1 )
    {
        nY += ( height()-pixMqs->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixMqs->m_pixmap );
}


class GMImageHmq
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgMqs( new GMImageHmq )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgMqs( new GMImageHmq )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgMqs( new GMImageHmq )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgMqs;
}

void GMImage::load ( const GString& strPath )
{
    imgMqs->m_imageOrg.load ( strPath );
    imgMqs->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgMqs->m_imageOrg.width(), imgMqs->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgMqs->m_imageOrg = img;
    imgMqs->m_imageScaled = img;
    GMItem::setSize ( imgMqs->m_imageOrg.width(), imgMqs->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if( width() != imgMqs->m_imageScaled.width() 
        || height() != imgMqs->m_imageScaled.height() )
    {
        imgMqs->m_imageScaled = imgMqs->m_imageOrg.smoothScale ( width(), height() );
    }
    
    int nX = x(), nY = y();
    if( width() > imgMqs->m_imageScaled.width()+1 )
    {
        nX += (width()-imgMqs->m_imageScaled.width() )/2;
    }
    if( height() > imgMqs->m_imageScaled.height()+1 )
    {
        nY += (height()-imgMqs->m_imageScaled.height() )/2;
    }
    p.drawImage ( nX, nY, imgMqs->m_imageScaled, 0, 0, width(), height() );
}

class GMTextHmq
{
public:
    GMTextHmq() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtMqs( new GMTextHmq )
{
    txtMqs->m_font = GFont ( "Sans", 20 );
    txtMqs->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtMqs->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtMqs( new GMTextHmq )
{
    txtMqs->m_str = str;
    txtMqs->m_font = GFont ( "Sans", 20 );
    txtMqs->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtMqs->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtMqs->m_font );
    txtMqs->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtMqs->m_str );
    setSize ( txtMqs->m_singleLineSize.width(), txtMqs->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtMqs;
}

void GMText::setColor ( const GColor& color )
{
    txtMqs->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtMqs->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtMqs->m_str;
}

GSize GMText::getSingleLineSize()
{
    return txtMqs->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtMqs->m_font );
    p.setPen ( txtMqs->m_color );
    p.drawText ( x(), y(), width(), height(), txtMqs->m_nFlags, txtMqs->m_str );
}

void GMText::setText ( const GString& str )
{
    txtMqs->m_str = str;
    GFontMetrics fontMetrics ( txtMqs->m_font );
    txtMqs->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtMqs->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtMqs->m_font = font;
    GFontMetrics fontMetrics ( txtMqs->m_font );
    txtMqs->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtMqs->m_str );
}

class GMScrollTextHmq
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stMqs( new GMScrollTextHmq )
{
    stMqs->m_pTimer = new GTimer ( this );
    connect ( stMqs->m_pTimer, stMqs->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stMqs->m_bIsResetX = true;
    stMqs->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stMqs( new GMScrollTextHmq )
{
    stMqs->m_pTimer = new GTimer ( this );
    connect ( stMqs->m_pTimer, stMqs->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stMqs->m_bIsResetX = true;
    stMqs->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stMqs;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stMqs->m_pTimer->start ( msec );
        stMqs->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stMqs->m_pTimer->stop();
        stMqs->m_bIsResetX = true;
        update();
        stMqs->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stMqs->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtMqs->m_singleLineSize.width() >width() )
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
            || stMqs->m_bShouldSroll == false )
    {
        return ;
    }
    
    update();
}


// nice day ^_^
// for fun ^_^
