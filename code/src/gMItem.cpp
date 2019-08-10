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

class GMItemLMQ
{
public:
    GMItemLMQ(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) 
    {}
    
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iHlo( new GMItemLMQ )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iHlo->m_pForm = form;
    setVisible ( true );
    iHlo->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iHlo;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iHlo->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iHlo->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iHlo->m_rect.width();
}
int GMItem::z() const
{
    return iHlo->m_nZ;
}
int GMItem::y() const
{
    return iHlo->m_rect.y();
}
int GMItem::x() const
{
    return iHlo->m_rect.x();
}
int GMItem::height() const
{
    return iHlo->m_rect.height();
}
int GMItem::right() const
{
    return iHlo->m_rect.right();
}
int GMItem::bottom() const
{
    return iHlo->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iHlo->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iHlo->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iHlo->m_pForm;
}
bool GMItem::isVisible()
{
    return iHlo->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iHlo->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iHlo->m_bgColor = c;
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
    if ( !iHlo->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iHlo->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iHlo->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iHlo->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iHlo->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iHlo->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iHlo->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iHlo->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iHlo->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iHlo->m_rect.moveBy ( x, y );
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

class GMCtrlItemLMQ
{
public:
    GMCtrlItemLMQ() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciHlo( new GMCtrlItemLMQ )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciHlo->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciHlo;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciHlo->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciHlo->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciHlo->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciHlo->m_nTabIndex;
}
bool GMCtrlItem::keyPressEvent ( GKeyEvent* )
{
    return false;
}

void GMCtrlItem::setFocus()
{
    if ( !isVisible() )
    {
        WARNING ( "item[%s] is not visible\n", name() );
        return;
    }
    else if ( !isFocusEnabled() )
    {
        WARNING ( "item[%s] is not focus-enabled\n", name() );
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
    return ciHlo->m_bIsHasFocus;
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
    ciHlo->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciHlo->m_bIsHasFocus = true;
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

class GMPixmapLMQ
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixHlo( new GMPixmapLMQ )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixHlo( new GMPixmapLMQ )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixHlo( new GMPixmapLMQ )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixHlo;
}

void GMPixmap::load ( const GString& strPath )
{
    pixHlo->m_pixmap.load ( strPath );
    setSize ( pixHlo->m_pixmap.width(), pixHlo->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixHlo->m_pixmap = pm;
    setSize ( pixHlo->m_pixmap.width(), pixHlo->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixHlo->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixHlo->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixHlo->m_pixmap.width() +1 )
    {
        nX += ( width()-pixHlo->m_pixmap.width() ) /2;
    }
    if ( height() > pixHlo->m_pixmap.height() +1 )
    {
        nY += ( height()-pixHlo->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixHlo->m_pixmap );
}


class GMImageLMQ
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgHlo( new GMImageLMQ )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgHlo( new GMImageLMQ )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgHlo( new GMImageLMQ )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgHlo;
}

void GMImage::load ( const GString& strPath )
{
    imgHlo->m_imageOrg.load ( strPath );
    imgHlo->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgHlo->m_imageOrg.width(), imgHlo->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgHlo->m_imageOrg = img;
    imgHlo->m_imageScaled = img;
    GMItem::setSize ( imgHlo->m_imageOrg.width(), imgHlo->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if ( width() > imgHlo->m_imageScaled.width()
            || height() > imgHlo->m_imageScaled.height() )
    {
        imgHlo->m_imageScaled = imgHlo->m_imageOrg.smoothScale ( width(), height() );
    }

    int nX = x(), nY = y();
    int off = width()-imgHlo->m_imageScaled.width();
    if ( 1 < off )
    {
        nX += off /2;
    }
    off =  height()-imgHlo->m_imageScaled.height();
    if ( 1 < off )
    {
        nY += off /2;
    }
    p.drawImage ( nX, nY, imgHlo->m_imageScaled, 0, 0, width(), height() );
}

class GMTextLMQ
{
public:
    GMTextLMQ() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtHlo( new GMTextLMQ )
{
    txtHlo->m_font = GFont ( "Sans", 20 );
    txtHlo->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtHlo->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtHlo( new GMTextLMQ )
{
    txtHlo->m_str = str;
    txtHlo->m_font = GFont ( "Sans", 20 );
    txtHlo->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtHlo->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtHlo->m_font );
    txtHlo->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtHlo->m_str );
    setSize ( txtHlo->m_singleLineSize.width(), txtHlo->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtHlo;
}

void GMText::setColor ( const GColor& color )
{
    txtHlo->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtHlo->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtHlo->m_str;
}

GSize GMText::getSingleLineSize()
{
    return txtHlo->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtHlo->m_font );
    p.setPen ( txtHlo->m_color );
    p.drawText ( x(), y(), width(), height(), txtHlo->m_nFlags, txtHlo->m_str );
}

void GMText::setText ( const GString& str )
{
    txtHlo->m_str = str;
    GFontMetrics fontMetrics ( txtHlo->m_font );
    txtHlo->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtHlo->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtHlo->m_font = font;
    GFontMetrics fontMetrics ( txtHlo->m_font );
    txtHlo->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtHlo->m_str );
}

class GMScrollTextLMQ
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stHlo( new GMScrollTextLMQ )
{
    stHlo->m_pTimer = new GTimer ( this );
    connect ( stHlo->m_pTimer, stHlo->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stHlo->m_bIsResetX = true;
    stHlo->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stHlo( new GMScrollTextLMQ )
{
    stHlo->m_pTimer = new GTimer ( this );
    connect ( stHlo->m_pTimer, stHlo->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stHlo->m_bIsResetX = true;
    stHlo->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stHlo;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stHlo->m_pTimer->start ( msec );
        stHlo->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stHlo->m_pTimer->stop();
        stHlo->m_bIsResetX = true;
        update();
        stHlo->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stHlo->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtHlo->m_singleLineSize.width() >width() )
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
            || stHlo->m_bShouldSroll == false )
    {
        return ;
    }
    
    update();
}


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
