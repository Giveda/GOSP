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

#include "gCtrlForm.h"
#include "gCtrlView.h"
#include "gCtrlStyle.h"
#include <gEvent.h>
#include <gConstDefine.h>
#include <gGlobal.h>

extern GCtrlView *g_ws;

class GMItemList : public GPtrList<GMItem>
{
protected:
    virtual int compareItems ( GMItem* item1, GMItem* item2 );
};

class GMCtrlItemList : public GPtrList<GMCtrlItem>
{
protected:
    virtual int compareItems ( GMCtrlItem* item1, GMCtrlItem* item2 );
};

int GMItemList::compareItems ( GMItem* p1, GMItem* p2 )
{
    if ( p1->z() == p2->z() )
    {
        return 0;
    }
    else if ( p1->z() > p2->z() )
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int GMCtrlItemList::compareItems ( GMCtrlItem* p1, GMCtrlItem* p2 )
{
    if ( p1->tabIndex() == p2->tabIndex() )
    {
        return 0;
    }
    else if ( p1->tabIndex() > p2->tabIndex() )
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

class GCtrlFormHmq
{
public:
    GCtrlFormHmq() 
    : m_pItemWithFocus ( NULL ), m_bgColor ( gRgba ( 0, 0, 0, 0 ) ), m_bIsVisible ( true ), m_eFocusMode ( GCtrlForm::Auto_Focus ), m_bIsFocusEnabled ( true )
    {}

    GMItemList m_itemList;
    GMCtrlItemList m_ctrlItemList;
    GRect m_rect;
    GPixmap m_pixBg;
    GPointer<GMCtrlItem> m_pItemWithFocus;
    GColor m_bgColor;
    bool m_bIsVisible;
    GCtrlForm::enumFocusMode m_eFocusMode;
    bool m_bIsFocusEnabled;
    int m_nZ;
};

GCtrlForm::GCtrlForm ( GCtrlForm* parent, const char* name )
    :GObject ( parent, name ),  frmMqs ( new GCtrlFormHmq )
{
    g_ws->appendForm ( this );
    setGeometry ( 0, 0, g_ws->width(), g_ws->height() );
    frmMqs->m_nZ = 1;
    frmMqs->m_pItemWithFocus = NULL;
    setVisible ( true );
    setFocusMode ( Auto_Focus );
}

GCtrlForm::~GCtrlForm()
{
    g_ws->removeForm ( this );

    delete frmMqs;
}

bool GCtrlForm::fwKeyPress ( GKeyEvent* e )
{
    if ( frmMqs->m_pItemWithFocus )
    {
        if ( true == frmMqs->m_pItemWithFocus->fwKeyPress ( e ) )
        {
            return true;
        }

#if 1
        GMCtrlItem* pParent = ( GMCtrlItem* ) ( frmMqs->m_pItemWithFocus->parent() );
        while ( pParent )
        {
            if ( true == pParent->fwKeyPress ( e ) )
            {
                return true;
            }

            pParent = ( GMCtrlItem* ) pParent->parent();
        }
#endif

        if ( Auto_Focus == getFocusMode() )
        {
            int nIndex = frmMqs->m_pItemWithFocus->tabIndex()-1;
            GMCtrlItem* pFrom = frmMqs->m_ctrlItemList.at ( nIndex );

            switch ( e->key() )
            {
            case Giveda::Key_Up:
            case Giveda::Key_Left:
                nIndex--;
                while ( nIndex>=0 )
                {
                    if ( frmMqs->m_ctrlItemList.at ( nIndex )->isVisible() && frmMqs->m_ctrlItemList.at ( nIndex )->isFocusEnabled() )
                    {
                        changeFocus ( pFrom, frmMqs->m_ctrlItemList.at ( nIndex ) );
                        break;
                    }

                    nIndex--;
                }
                break;
            case Giveda::Key_Down:
            case Giveda::Key_Right:
                nIndex++;
                while ( ( unsigned int ) nIndex<frmMqs->m_ctrlItemList.count() )
                {
                    if ( frmMqs->m_ctrlItemList.at ( nIndex )->isVisible() && frmMqs->m_ctrlItemList.at ( nIndex )->isFocusEnabled() )
                    {
                        changeFocus ( pFrom, frmMqs->m_ctrlItemList.at ( nIndex ) );
                        break;
                    }

                    nIndex++;
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        unsigned int i = 0;
        while ( i<frmMqs->m_ctrlItemList.count() )
        {
            if ( frmMqs->m_ctrlItemList.at ( i )->isVisible() && frmMqs->m_ctrlItemList.at ( i )->isFocusEnabled() )
            {
                setFocusToItem ( frmMqs->m_ctrlItemList.at ( i ) );
                break;
            }

            i++;
        }
    }

    bool bRet = keyPressEvent ( e );
    return bRet;
}

void GCtrlForm::paintEvent ( GPainter &p )
{
    if ( !frmMqs->m_pItemWithFocus && hasFocus() )
    {
        initFocus();
    }

    if ( frmMqs->m_pixBg.isNull() )
    {
        p.fillRect ( 0, 0, width(), height(), GBrush ( frmMqs->m_bgColor ) );
    }
    else
    {
        p.drawPixmap ( 0, 0, frmMqs->m_pixBg );
    }

    for ( GMItem* pItem = frmMqs->m_itemList.first(); pItem!=NULL; pItem = frmMqs->m_itemList.next() )
    {
        if ( !pItem->isVisible() )
        {
            continue;
        }

        p.save();
        if ( pItem->isComplex() )
        {
            p.translate ( pItem->x(), pItem->y() );
            pItem->draw ( p );
        }
        else
        {
            pItem->draw ( p );
        }

        p.restore();
    }
}

void GCtrlForm::setX ( int x )
{
    frmMqs->m_rect.moveBy ( x-this->x(), 0 );
}

void GCtrlForm::setY ( int y )
{
    frmMqs->m_rect.moveBy ( 0, y-this->y() );
}

void GCtrlForm::setZ ( int z )
{
    frmMqs->m_nZ = z;
    g_ws->inSort ( this );
}

void GCtrlForm::setWidth ( int w )
{
    frmMqs->m_rect.setWidth ( w );
}

void GCtrlForm::setHeight ( int h )
{
    frmMqs->m_rect.setHeight ( h );
}

void GCtrlForm::setPosition ( int x, int y )
{
    frmMqs->m_rect.moveTopLeft ( GPoint ( x,y ) );
}

void GCtrlForm::setSize ( int w, int h )
{
    frmMqs->m_rect.setSize ( GSize ( w, h ) );
}

void GCtrlForm::setGeometry ( int x, int y, int w, int h )
{
    frmMqs->m_rect.setRect ( x, y, w, h );
}

void GCtrlForm::loadBackgroundPixmap ( const GString& strPic )
{
    frmMqs->m_pixBg.load ( strPic );
    frmMqs->m_rect.setSize ( GSize ( frmMqs->m_pixBg.width(), frmMqs->m_pixBg.height() ) );
}

void GCtrlForm::loadBackgroundPixmap ( const GPixmap& px )
{
    frmMqs->m_pixBg = px;
    frmMqs->m_rect.setSize ( GSize ( frmMqs->m_pixBg.width(), frmMqs->m_pixBg.height() ) );
}

void GCtrlForm::appendItem ( GMItem* item )
{
    frmMqs->m_itemList.inSort ( item );
    if ( item->isCtrlItem() )
    {
        frmMqs->m_ctrlItemList.inSort ( ( GMCtrlItem* ) item );
    }
}

void GCtrlForm::setFocus()
{
    g_ws->setFocusToFrm ( this );
}

void GCtrlForm::setFocusToItem ( GMCtrlItem* pItem )
{
    if ( frmMqs->m_pItemWithFocus )
    {
        if ( pItem != frmMqs->m_pItemWithFocus )
        {
            frmMqs->m_pItemWithFocus->emitLoseFocus();
            frmMqs->m_pItemWithFocus->update();
        }
        else
        {
            return ;
        }
    }

    frmMqs->m_pItemWithFocus = pItem;

    if ( frmMqs->m_pItemWithFocus )
    {
        frmMqs->m_pItemWithFocus->emitGetFocus();
        frmMqs->m_pItemWithFocus->update();
    }
}

void GCtrlForm::changeFocus ( GMCtrlItem* pFrom, GMCtrlItem* pTo )
{
    pFrom->emitLoseFocus();
    pFrom->update();

    frmMqs->m_pItemWithFocus = pTo;

    frmMqs->m_pItemWithFocus->emitGetFocus();
    frmMqs->m_pItemWithFocus->update();
}

GMCtrlItem* GCtrlForm::getFocusItem()
{
    return frmMqs->m_pItemWithFocus;
}

unsigned int GCtrlForm::getMaxTabIndex()
{
    GMCtrlItem* pLastItem = frmMqs->m_ctrlItemList.getLast();
    if ( pLastItem )
    {
        return pLastItem->tabIndex();
    }
    else
    {
        return 0;
    }
}


void GCtrlForm::update ( GRect r )
{
    r.moveBy ( x(), y() );
    r = r.intersect ( rect() );
    g_ws->update ( r );
}

void GCtrlForm::update()
{
    g_ws->update ( frmMqs->m_rect );
}

void GCtrlForm::show()
{
    setVisible ( true );
    g_ws->update ( frmMqs->m_rect );
    setFocus();
}

void GCtrlForm::hide()
{
    setVisible ( false );
    g_ws->update ( frmMqs->m_rect );
    g_ws->setFocusToFrm ( NULL );
}

bool GCtrlForm::hasFocus()
{
    return this == g_ws->getFocusFrm() ? true:false;
}

void GCtrlForm::initFocus()
{
    unsigned int nCount = frmMqs->m_ctrlItemList.count();
    unsigned int i=0;
    while ( i<nCount )
    {
        if ( frmMqs->m_ctrlItemList.at ( i )->isVisible() && frmMqs->m_ctrlItemList.at ( i )->isFocusEnabled() )
        {
            frmMqs->m_pItemWithFocus = frmMqs->m_ctrlItemList.at ( i );
            frmMqs->m_pItemWithFocus->update();
            frmMqs->m_pItemWithFocus->emitGetFocus();
            break;
        }

        i++;
    }
}


GCtrlDefaultAppStyle* GCtrlForm::getDefaultAppStyle()
{
    return g_ws->appStyle();
}

GCtrlView* GCtrlForm::view()
{
    return g_ws;
}

void GCtrlForm::getFocus()
{
    if ( frmMqs->m_pItemWithFocus )
    {
        frmMqs->m_pItemWithFocus->emitGetFocus();
    }
}

void GCtrlForm::loseFocus()
{
    if ( frmMqs->m_pItemWithFocus )
    {
        frmMqs->m_pItemWithFocus->emitLoseFocus();
    }
}

int GCtrlForm::x() const
{
    return frmMqs->m_rect.x();
}

int GCtrlForm::y() const
{
    return frmMqs->m_rect.y();
}
int GCtrlForm::z() const
{
    return frmMqs->m_nZ;
}
int GCtrlForm::width() const
{
    return frmMqs->m_rect.width();
}
int GCtrlForm::height() const
{
    return frmMqs->m_rect.height();
}
GRect GCtrlForm::rect() const
{
    return frmMqs->m_rect;
}
void GCtrlForm::moveBy ( int x, int y )
{
    frmMqs->m_rect.moveBy ( x, y );
}
void GCtrlForm::setPaletteBackgroundColor ( const GColor& c )
{
    frmMqs->m_bgColor = c;
}
bool GCtrlForm::isVisible()
{
    return frmMqs->m_bIsVisible;
}
void GCtrlForm::setVisible ( bool b )
{
    frmMqs->m_bIsVisible = b;
}
void GCtrlForm::setFocusMode ( GCtrlForm::enumFocusMode mode )
{
    frmMqs->m_eFocusMode = mode;
}
GCtrlForm::enumFocusMode GCtrlForm::getFocusMode()
{
    return frmMqs->m_eFocusMode;
}
bool GCtrlForm::isFocusEnabled()
{
    return frmMqs->m_bIsFocusEnabled;
}

void GCtrlForm::setFocusEnabled ( bool b )
{
    frmMqs->m_bIsFocusEnabled = b;
}

bool GCtrlForm::keyPressEvent ( GKeyEvent* )
{
    return false;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
