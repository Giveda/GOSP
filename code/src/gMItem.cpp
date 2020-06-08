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

class GMItemSelf
{
public:
    GMItemSelf(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) 
    {}
    
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iSpp( new GMItemSelf )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iSpp->m_pForm = form;
    setVisible ( true );
    iSpp->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iSpp;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iSpp->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iSpp->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iSpp->m_rect.width();
}
int GMItem::z() const
{
    return iSpp->m_nZ;
}
int GMItem::y() const
{
    return iSpp->m_rect.y();
}
int GMItem::x() const
{
    return iSpp->m_rect.x();
}
int GMItem::height() const
{
    return iSpp->m_rect.height();
}
int GMItem::right() const
{
    return iSpp->m_rect.right();
}
int GMItem::bottom() const
{
    return iSpp->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iSpp->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iSpp->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iSpp->m_pForm;
}
bool GMItem::isVisible()
{
    return iSpp->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iSpp->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iSpp->m_bgColor = c;
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
    if ( !iSpp->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iSpp->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iSpp->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iSpp->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iSpp->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iSpp->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iSpp->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iSpp->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iSpp->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iSpp->m_rect.moveBy ( x, y );
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

class GMCtrlItemSelf
{
public:
    GMCtrlItemSelf() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciSpp( new GMCtrlItemSelf )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciSpp->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciSpp;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciSpp->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciSpp->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciSpp->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciSpp->m_nTabIndex;
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
    return ciSpp->m_bIsHasFocus;
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
    ciSpp->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciSpp->m_bIsHasFocus = true;
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

class GMPixmapSelf
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixSpp( new GMPixmapSelf )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixSpp( new GMPixmapSelf )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixSpp( new GMPixmapSelf )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixSpp;
}

void GMPixmap::load ( const GString& strPath )
{
    pixSpp->m_pixmap.load ( strPath );
    setSize ( pixSpp->m_pixmap.width(), pixSpp->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixSpp->m_pixmap = pm;
    setSize ( pixSpp->m_pixmap.width(), pixSpp->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixSpp->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixSpp->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixSpp->m_pixmap.width() +1 )
    {
        nX += ( width()-pixSpp->m_pixmap.width() ) /2;
    }
    if ( height() > pixSpp->m_pixmap.height() +1 )
    {
        nY += ( height()-pixSpp->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixSpp->m_pixmap );
}


class GMImageSelf
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgSpp( new GMImageSelf )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgSpp( new GMImageSelf )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgSpp( new GMImageSelf )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgSpp;
}

void GMImage::load ( const GString& strPath )
{
    imgSpp->m_imageOrg.load ( strPath );
    imgSpp->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgSpp->m_imageOrg.width(), imgSpp->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgSpp->m_imageOrg = img;
    imgSpp->m_imageScaled = img;
    GMItem::setSize ( imgSpp->m_imageOrg.width(), imgSpp->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if( width() != imgSpp->m_imageScaled.width() 
        || height() != imgSpp->m_imageScaled.height() )
    {
        imgSpp->m_imageScaled = imgSpp->m_imageOrg.smoothScale ( width(), height() );
    }
    
    int nX = x(), nY = y();
    if( width() > imgSpp->m_imageScaled.width()+1 )
    {
        nX += (width()-imgSpp->m_imageScaled.width() )/2;
    }
    if( height() > imgSpp->m_imageScaled.height()+1 )
    {
        nY += (height()-imgSpp->m_imageScaled.height() )/2;
    }
    p.drawImage ( nX, nY, imgSpp->m_imageScaled, 0, 0, width(), height() );
}

class GMTextSelf
{
public:
    GMTextSelf() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtSpp( new GMTextSelf )
{
    txtSpp->m_font = GFont ( "Sans", 20 );
    txtSpp->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtSpp->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtSpp( new GMTextSelf )
{
    txtSpp->m_str = str;
    txtSpp->m_font = GFont ( "Sans", 20 );
    txtSpp->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtSpp->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtSpp->m_font );
    txtSpp->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtSpp->m_str );
    setSize ( txtSpp->m_singleLineSize.width(), txtSpp->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtSpp;
}

void GMText::setColor ( const GColor& color )
{
    txtSpp->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtSpp->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtSpp->m_str;
}

GSize GMText::getSingleLineSize()
{
    return txtSpp->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtSpp->m_font );
    p.setPen ( txtSpp->m_color );
    p.drawText ( x(), y(), width(), height(), txtSpp->m_nFlags, txtSpp->m_str );
}

void GMText::setText ( const GString& str )
{
    txtSpp->m_str = str;
    GFontMetrics fontMetrics ( txtSpp->m_font );
    txtSpp->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtSpp->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtSpp->m_font = font;
    GFontMetrics fontMetrics ( txtSpp->m_font );
    txtSpp->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtSpp->m_str );
}

class GMScrollTextSelf
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stSpp( new GMScrollTextSelf )
{
    stSpp->m_pTimer = new GTimer ( this );
    connect ( stSpp->m_pTimer, stSpp->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stSpp->m_bIsResetX = true;
    stSpp->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stSpp( new GMScrollTextSelf )
{
    stSpp->m_pTimer = new GTimer ( this );
    connect ( stSpp->m_pTimer, stSpp->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stSpp->m_bIsResetX = true;
    stSpp->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stSpp;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stSpp->m_pTimer->start ( msec );
        stSpp->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stSpp->m_pTimer->stop();
        stSpp->m_bIsResetX = true;
        update();
        stSpp->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stSpp->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtSpp->m_singleLineSize.width() >width() )
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
            || stSpp->m_bShouldSroll == false )
    {
        return ;
    }
    
    update();
}


// a nice day ^_^
// a nice day ^_^
