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

class GMItemPrivate
{
public:
    GMItemPrivate(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) {}
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iPriv( new GMItemPrivate )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iPriv->m_pForm = form;
    setVisible ( true );
    iPriv->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iPriv;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iPriv->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iPriv->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iPriv->m_rect.width();
}
int GMItem::z() const
{
    return iPriv->m_nZ;
}
int GMItem::y() const
{
    return iPriv->m_rect.y();
}
int GMItem::x() const
{
    return iPriv->m_rect.x();
}
int GMItem::height() const
{
    return iPriv->m_rect.height();
}
int GMItem::right() const
{
    return iPriv->m_rect.right();
}
int GMItem::bottom() const
{
    return iPriv->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iPriv->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iPriv->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iPriv->m_pForm;
}
bool GMItem::isVisible()
{
    return iPriv->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iPriv->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iPriv->m_bgColor = c;
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
    if ( !iPriv->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iPriv->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iPriv->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iPriv->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iPriv->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iPriv->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iPriv->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iPriv->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iPriv->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iPriv->m_rect.moveBy ( x, y );
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

class GMCtrlItemPrivate
{
public:
    GMCtrlItemPrivate() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciPriv( new GMCtrlItemPrivate )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciPriv->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciPriv;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciPriv->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciPriv->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciPriv->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciPriv->m_nTabIndex;
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
    return ciPriv->m_bIsHasFocus;
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
    ciPriv->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciPriv->m_bIsHasFocus = true;
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

class GMPixmapPrivate
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixPriv( new GMPixmapPrivate )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixPriv( new GMPixmapPrivate )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixPriv( new GMPixmapPrivate )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixPriv;
}

void GMPixmap::load ( const GString& strPath )
{
    pixPriv->m_pixmap.load ( strPath );
    setSize ( pixPriv->m_pixmap.width(), pixPriv->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixPriv->m_pixmap = pm;
    setSize ( pixPriv->m_pixmap.width(), pixPriv->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixPriv->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixPriv->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixPriv->m_pixmap.width() +1 )
    {
        nX += ( width()-pixPriv->m_pixmap.width() ) /2;
    }
    if ( height() > pixPriv->m_pixmap.height() +1 )
    {
        nY += ( height()-pixPriv->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixPriv->m_pixmap );
}


class GMImagePrivate
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgPriv( new GMImagePrivate )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgPriv( new GMImagePrivate )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgPriv( new GMImagePrivate )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgPriv;
}

void GMImage::load ( const GString& strPath )
{
    imgPriv->m_imageOrg.load ( strPath );
    imgPriv->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgPriv->m_imageOrg.width(), imgPriv->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgPriv->m_imageOrg = img;
    imgPriv->m_imageScaled = img;
    GMItem::setSize ( imgPriv->m_imageOrg.width(), imgPriv->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if ( width() > imgPriv->m_imageScaled.width()
            || height() > imgPriv->m_imageScaled.height() )
    {
        imgPriv->m_imageScaled = imgPriv->m_imageOrg.smoothScale ( width(), height() );
    }

    int nX = x(), nY = y();
    int off = width()-imgPriv->m_imageScaled.width();
    if ( 1 < off )
    {
        nX += off /2;
    }
    off =  height()-imgPriv->m_imageScaled.height();
    if ( 1 < off )
    {
        nY += off /2;
    }
    p.drawImage ( nX, nY, imgPriv->m_imageScaled, 0, 0, width(), height() );
}

class GMTextPrivate
{
public:
    GMTextPrivate() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtPriv( new GMTextPrivate )
{
    txtPriv->m_font = GFont ( "Sans", 20 );
    txtPriv->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtPriv->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtPriv( new GMTextPrivate )
{
    txtPriv->m_str = str;
    txtPriv->m_font = GFont ( "Sans", 20 );
    txtPriv->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtPriv->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtPriv->m_font );
    txtPriv->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtPriv->m_str );
    setSize ( txtPriv->m_singleLineSize.width(), txtPriv->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtPriv;
}

void GMText::setColor ( const GColor& color )
{
    txtPriv->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtPriv->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtPriv->m_str;
}
GSize GMText::getSingleLineSize()
{
    return txtPriv->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtPriv->m_font );
    p.setPen ( txtPriv->m_color );
    p.drawText ( x(), y(), width(), height(), txtPriv->m_nFlags, txtPriv->m_str );
}

void GMText::setText ( const GString& str )
{
    txtPriv->m_str = str;
    GFontMetrics fontMetrics ( txtPriv->m_font );
    txtPriv->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtPriv->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtPriv->m_font = font;
    GFontMetrics fontMetrics ( txtPriv->m_font );
    txtPriv->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtPriv->m_str );
}

class GMScrollTextPrivate
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stPriv( new GMScrollTextPrivate )
{
    stPriv->m_pTimer = new GTimer ( this );
    connect ( stPriv->m_pTimer, stPriv->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stPriv->m_bIsResetX = true;
    stPriv->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stPriv( new GMScrollTextPrivate )
{
    stPriv->m_pTimer = new GTimer ( this );
    connect ( stPriv->m_pTimer, stPriv->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stPriv->m_bIsResetX = true;
    stPriv->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stPriv;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stPriv->m_pTimer->start ( msec );
        stPriv->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stPriv->m_pTimer->stop();
        stPriv->m_bIsResetX = true;
        update();
        stPriv->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stPriv->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtPriv->m_singleLineSize.width() >width() )
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
            || stPriv->m_bShouldSroll == false )
    {
        return ;
    }
    update();
}


// have a nice day ^_^
