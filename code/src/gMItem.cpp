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

class GMItemFles
{
public:
    GMItemFles(): m_bgColor ( gRgba ( 0, 0, 0, 0 ) ) 
    {}
    
    GRect m_rect;
    int m_nZ;
    GCtrlForm* m_pForm;
    bool m_bIsVisible;
    GColor m_bgColor;
    GPixmap m_pixBg;
};

GMItem::GMItem ( GCtrlForm* form, GMItem* parent, const char* name )
    :GObject ( parent, name ), iFhl( new GMItemFles )
{
    setGeometry ( 0, 0, form->width() /2, form->height() /2 );
    iFhl->m_pForm = form;
    setVisible ( true );
    iFhl->m_nZ = 1;
#if 0
    if ( parent )
    {
        parent->addChild ( this );
    }
#endif
}

GMItem::~GMItem()
{
    delete iFhl;
}

GCtrlDefaultAppStyle* GMItem::getDefaultAppStyle()
{
    return form()->view()->appStyle();
}
void GMItem::setWidth ( int w )
{
    iFhl->m_rect.setWidth ( w );
}
void GMItem::setHeight ( int h )
{
    iFhl->m_rect.setHeight ( h );
}
int GMItem::width() const
{
    return iFhl->m_rect.width();
}
int GMItem::z() const
{
    return iFhl->m_nZ;
}
int GMItem::y() const
{
    return iFhl->m_rect.y();
}
int GMItem::x() const
{
    return iFhl->m_rect.x();
}
int GMItem::height() const
{
    return iFhl->m_rect.height();
}
int GMItem::right() const
{
    return iFhl->m_rect.right();
}
int GMItem::bottom() const
{
    return iFhl->m_rect.bottom();
}
GRect GMItem::rect() const
{
    return iFhl->m_rect;
}
void GMItem::setSize ( int w, int h )
{
    iFhl->m_rect.setSize ( GSize ( w, h ) );
}
bool GMItem::isCtrlItem()
{
    return false;
}
GCtrlForm* GMItem::form()
{
    return iFhl->m_pForm;
}
bool GMItem::isVisible()
{
    return iFhl->m_bIsVisible;
}
void GMItem::setVisible ( bool b )
{
    iFhl->m_bIsVisible = b;
}
void GMItem::setPaletteBackgroundColor ( const GColor& c )
{
    iFhl->m_bgColor = c;
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
    if ( !iFhl->m_pixBg.isNull() )
    {
        p.drawPixmap ( 0, 0, iFhl->m_pixBg );
    }
#if 0
    else
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( iFhl->m_bgColor ) );
    }
#endif

    paintEvent ( p );
}

void GMItem::loadBackgroundPixmap ( const GString& strPic )
{
    iFhl->m_pixBg.load ( strPic );
}

void GMItem::setX ( int x )
{
    iFhl->m_rect.moveBy ( x-this->x(), 0 );
}

void GMItem::setY ( int y )
{
    iFhl->m_rect.moveBy ( 0, y-this->y() );
}

void GMItem::setZ ( int z )
{
    iFhl->m_nZ = z;
}

void GMItem::setPosition ( int x, int y )
{
    iFhl->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GMItem::setGeometry ( int x, int y, int w, int h )
{
    iFhl->m_rect.setRect ( x, y, w, h );
#if 0
    for ( GMItem* pItem = m_listChildren.first(); pItem!=NULL; pItem = m_listChildren.next() )
    {
        pItem->moveBy ( x-this->x(), y-this->y() );
    }
#endif
}

void GMItem::moveBy ( int x, int y )
{
    iFhl->m_rect.moveBy ( x, y );
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

class GMCtrlItemFles
{
public:
    GMCtrlItemFles() : m_bIsHasFocus ( false ), m_bIsFocusEnabled ( true ) {}
    bool m_bIsHasFocus;
    bool m_bIsFocusEnabled;
    unsigned int m_nTabIndex;
};

GMCtrlItem::GMCtrlItem ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), ciFhl( new GMCtrlItemFles )
{
    if ( parent )
    {
        setTabIndex ( ( ( GMCtrlItem* ) parent )->tabIndex() );
    }
    else
    {
        setTabIndex ( form->getMaxTabIndex() +1 );
    }
    
    ciFhl->m_bIsHasFocus = false;
    setFocusEnabled( true );
}

GMCtrlItem::~GMCtrlItem()
{
    delete ciFhl;
}

bool GMCtrlItem::isCtrlItem()
{
    return true;
}
bool GMCtrlItem::isFocusEnabled()
{
    return ciFhl->m_bIsFocusEnabled;
}
void GMCtrlItem::setFocusEnabled ( bool b )
{
    ciFhl->m_bIsFocusEnabled = b;
}
void GMCtrlItem::setTabIndex ( unsigned int index )
{
    ciFhl->m_nTabIndex = index;
}
unsigned int GMCtrlItem::tabIndex()
{
    return ciFhl->m_nTabIndex;
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
    return ciFhl->m_bIsHasFocus;
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
    ciFhl->m_bIsHasFocus = false;
    loseFocus.emit();
}

void GMCtrlItem::emitGetFocus()
{
    ciFhl->m_bIsHasFocus = true;
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

class GMPixmapFles
{
public:
    GPixmap m_pixmap;
};

GMPixmap::GMPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixFhl( new GMPixmapFles )
{
}

GMPixmap::GMPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixFhl( new GMPixmapFles )
{
    load ( strPath );
}

GMPixmap::GMPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), pixFhl( new GMPixmapFles )
{
    setPixmap ( pm );
}

GMPixmap::~GMPixmap()
{
    delete pixFhl;
}

void GMPixmap::load ( const GString& strPath )
{
    pixFhl->m_pixmap.load ( strPath );
    setSize ( pixFhl->m_pixmap.width(), pixFhl->m_pixmap.height() );
}

void GMPixmap::setPixmap ( const GPixmap &pm )
{
    pixFhl->m_pixmap = pm;
    setSize ( pixFhl->m_pixmap.width(), pixFhl->m_pixmap.height() );
}

void GMPixmap::loadButNotAdjustSize ( const GString& strPath )
{
    pixFhl->m_pixmap.load ( strPath );
}

void GMPixmap::setPixmapButNotAdjustSize ( const GPixmap &pm )
{
    pixFhl->m_pixmap = pm;
}

void GMPixmap::paintEvent ( GPainter& p )
{
    int nX = x(), nY = y();
    if ( width() > pixFhl->m_pixmap.width() +1 )
    {
        nX += ( width()-pixFhl->m_pixmap.width() ) /2;
    }
    if ( height() > pixFhl->m_pixmap.height() +1 )
    {
        nY += ( height()-pixFhl->m_pixmap.height() ) /2;
    }
    p.drawPixmap ( nX, nY, pixFhl->m_pixmap );
}


class GMImageFles
{
public:
    GImage m_imageOrg;
    GImage m_imageScaled;
};

GMImage::GMImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgFhl( new GMImageFles )
{
}

GMImage::GMImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgFhl( new GMImageFles )
{
    load ( strPath );
}

GMImage::GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), imgFhl( new GMImageFles )
{
    setImage ( img );
}

GMImage::~GMImage()
{
    delete imgFhl;
}

void GMImage::load ( const GString& strPath )
{
    imgFhl->m_imageOrg.load ( strPath );
    imgFhl->m_imageScaled.load ( strPath );
    GMItem::setSize ( imgFhl->m_imageOrg.width(), imgFhl->m_imageOrg.height() );
}

void GMImage::setImage ( const GImage &img )
{
    imgFhl->m_imageOrg = img;
    imgFhl->m_imageScaled = img;
    GMItem::setSize ( imgFhl->m_imageOrg.width(), imgFhl->m_imageOrg.height() );
}

void GMImage::paintEvent ( GPainter& p )
{
    if ( width() > imgFhl->m_imageScaled.width()
            || height() > imgFhl->m_imageScaled.height() )
    {
        imgFhl->m_imageScaled = imgFhl->m_imageOrg.smoothScale ( width(), height() );
    }

    int nX = x(), nY = y();
    int off = width()-imgFhl->m_imageScaled.width();
    if ( 1 < off )
    {
        nX += off /2;
    }
    off =  height()-imgFhl->m_imageScaled.height();
    if ( 1 < off )
    {
        nY += off /2;
    }
    p.drawImage ( nX, nY, imgFhl->m_imageScaled, 0, 0, width(), height() );
}

class GMTextFles
{
public:
    GMTextFles() : m_singleLineSize ( 0, 0 ) {}
    GString m_str;
    GFont m_font;
    GColor m_color;
    int m_nFlags;
    GSize m_singleLineSize;
};

GMText::GMText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtFhl( new GMTextFles )
{
    txtFhl->m_font = GFont ( "Sans", 20 );
    txtFhl->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtFhl->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;
}

GMText::GMText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMItem ( form, parent, name ), txtFhl( new GMTextFles )
{
    txtFhl->m_str = str;
    txtFhl->m_font = GFont ( "Sans", 20 );
    txtFhl->m_color = GColor ( 0x10, 0x10, 0x10 );
    txtFhl->m_nFlags = Giveda::AlignLeft | Giveda::WordBreak;

    GFontMetrics fontMetrics ( txtFhl->m_font );
    txtFhl->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtFhl->m_str );
    setSize ( txtFhl->m_singleLineSize.width(), txtFhl->m_singleLineSize.height() );
}

GMText::~GMText()
{
    delete txtFhl;
}

void GMText::setColor ( const GColor& color )
{
    txtFhl->m_color = color;
}
void GMText::setTextFlags ( int nFlags )
{
    txtFhl->m_nFlags = nFlags;
}
GString GMText::text()
{
    return txtFhl->m_str;
}

GSize GMText::getSingleLineSize()
{
    return txtFhl->m_singleLineSize;
}

void GMText::paintEvent ( GPainter& p )
{
    p.setFont ( txtFhl->m_font );
    p.setPen ( txtFhl->m_color );
    p.drawText ( x(), y(), width(), height(), txtFhl->m_nFlags, txtFhl->m_str );
}

void GMText::setText ( const GString& str )
{
    txtFhl->m_str = str;
    GFontMetrics fontMetrics ( txtFhl->m_font );
    txtFhl->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtFhl->m_str );
}

void GMText::setFont ( const GFont & font )
{
    txtFhl->m_font = font;
    GFontMetrics fontMetrics ( txtFhl->m_font );
    txtFhl->m_singleLineSize = fontMetrics.size ( Giveda::SingleLine, txtFhl->m_str );
}

class GMScrollTextFles
{
public:
    GTimer* m_pTimer;
    bool m_bIsResetX;
    int m_nX;
    int m_nW;
    bool m_bShouldSroll;
};

GMScrollText::GMScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( form, parent, name ), stFhl( new GMScrollTextFles )
{
    stFhl->m_pTimer = new GTimer ( this );
    connect ( stFhl->m_pTimer, stFhl->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stFhl->m_bIsResetX = true;
    stFhl->m_bShouldSroll = false;
}

GMScrollText::GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMText ( str, form, parent, name ), stFhl( new GMScrollTextFles )
{
    stFhl->m_pTimer = new GTimer ( this );
    connect ( stFhl->m_pTimer, stFhl->m_pTimer->timeout, this, &GMScrollText::slotUpdate );
    stFhl->m_bIsResetX = true;
    stFhl->m_bShouldSroll = false;
}

GMScrollText::~GMScrollText()
{
    delete stFhl;
}

void GMScrollText::startScroll ( unsigned int msec )
{
    if ( !isScroll() && isNeedScroll() )
    {
        stFhl->m_pTimer->start ( msec );
        stFhl->m_bShouldSroll = true;
    }
}

void GMScrollText::stopScroll()
{
    if ( isScroll() )
    {
        stFhl->m_pTimer->stop();
        stFhl->m_bIsResetX = true;
        update();
        stFhl->m_bShouldSroll = false;
    }
}

bool GMScrollText::isScroll()
{
    return stFhl->m_pTimer->isActive();
}

bool GMScrollText::isNeedScroll()
{
    if ( txtFhl->m_singleLineSize.width() >width() )
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
            || stFhl->m_bShouldSroll == false )
    {
        return ;
    }
    
    update();
}


// have a nice day ^_^
