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

#include <gCtrlStyle.h>
#include "gCtrlListBox.h"
#include <gConstDefine.h>
#include <gGlobal.h>

GCtrlListBoxItem::GCtrlListBoxItem ( const GString& str, GCtrlForm* frm, GMItem* parent, const char* name )
    :GMCtrlItem ( frm, parent, name ), m_txt ( str, frm, this, "listBoxText" )
{
    m_txt.setTextFlags ( Giveda::AlignVCenter );
}

void GCtrlListBoxItem::paintEvent ( GPainter& p )
{
    m_txt.draw ( p );
}

bool GCtrlListBoxItem::fwKeyPressEvent ( GKeyEvent *e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        {
            GCtrlListBox* pBox = ( GCtrlListBox* ) parent();
            pBox->emitSelected ( this );
        }
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlListBoxItem::setGeometry ( int x, int y, int w, int h )
{
    GMItem::setGeometry ( x,y,w,h );
    m_txt.setSize ( width(), height() );
}

GCtrlListBoxPixmap::GCtrlListBoxPixmap ( const GPixmap& pm, const GString& str, GCtrlForm* frm, GMItem* parent, const char* name )
    :GCtrlListBoxItem ( str, frm, parent, name ), m_pix ( pm, frm, this, "listBoxPixmap" )
{
}

void GCtrlListBoxPixmap::paintEvent ( GPainter& p )
{
    m_pix.draw ( p );
    m_txt.draw ( p );
}

void GCtrlListBoxPixmap::setGeometry ( int x, int y, int w, int h )
{
    GMItem::setGeometry ( x,y,w,h );
    int nTmp = height() - m_pix.height();
    if ( nTmp > 1 )
    {
        m_pix.setY ( nTmp/2 );
    }
    m_txt.setGeometry ( m_pix.width(), 0,  width()-m_pix.width(), height() );
}

class GCtrlListBoxPrivate
{
public:
    GCtrlListBoxPrivate ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
        :m_imgFocusIn ( frm, parent, "listBoxFocusIn" ), m_imgFocusOut ( frm, parent, "listBoxFocusOut" ), m_timerForShowItemInfo ( lb ), m_itemTxtColor ( 255,255,255 ), m_itemTxtFont ( "Sans", 20 )
    {}
    GPtrList<GCtrlListBoxItem> m_itemList;
    GMImage m_imgFocusIn;
    GMImage m_imgFocusOut;
    GTimer m_timerForShowItemInfo;
    unsigned int m_nCurItemIndex;
    bool m_bIsNeedDoLayout;
    bool m_bIsNeedShowItemInfo;
    bool m_bSendHighLighted;
    unsigned int m_nDrawFrom;
    unsigned int m_nDrawTo;
    unsigned int m_nMaxWofItem;
    unsigned int m_nMaxHofItem;
    int m_nColumnNums;
    int m_nRowNums;
    GColor m_itemTxtColor;
    GFont m_itemTxtFont;
    bool m_bScrollEnabled;
    int m_nPreviousItemKey;
    int m_nNextItemKey;
};

GCtrlListBox::GCtrlListBox ( GCtrlForm* frm, GMItem* parent, const char* name )
    :GMContainerItem ( frm, parent, name )
{
    lbPriv = new GCtrlListBoxPrivate(frm, parent, this );
    frm->appendItem ( this );
    lbPriv->m_nDrawFrom = 0;
    lbPriv->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbPriv->m_timerForShowItemInfo,
              lbPriv->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbPriv->m_nCurItemIndex = 0;
    lbPriv->m_bIsNeedDoLayout = true;
    lbPriv->m_bIsNeedShowItemInfo = true;
    lbPriv->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbPriv->m_imgFocusIn.setImage ( pStyle->pixmap ( lbPriv->m_imgFocusIn.name() ) );
    lbPriv->m_imgFocusOut.setImage ( pStyle->pixmap ( lbPriv->m_imgFocusOut.name() ) );
    
    lbPriv->m_itemList.setAutoDelete ( true );

    lbPriv->m_bScrollEnabled = true;
    lbPriv->m_nPreviousItemKey = Giveda::Key_Up;
    lbPriv->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbPriv->m_itemList.clear();
    delete lbPriv;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbPriv->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbPriv->m_itemList.count()-1<lbPriv->m_nDrawTo ? lbPriv->m_itemList.count()-1 : lbPriv->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbPriv->m_bIsNeedDoLayout )
    {
        lbPriv->m_itemList.at ( lbPriv->m_nDrawFrom )->setGeometry ( 0, 0, lbPriv->m_nMaxWofItem, lbPriv->m_nMaxHofItem );
        lbPriv->m_itemList.at ( lbPriv->m_nDrawFrom )->getTxt()->setColor ( lbPriv->m_itemTxtColor );
        lbPriv->m_itemList.at ( lbPriv->m_nDrawFrom )->getTxt()->setFont ( lbPriv->m_itemTxtFont );
        int nBottomEdge = lbPriv->m_nMaxHofItem* ( lbPriv->m_nRowNums-1 );
        for ( int j=lbPriv->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbPriv->m_itemList.at ( j+1 );
            if ( lbPriv->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbPriv->m_itemList.at ( j )->x() +lbPriv->m_nMaxWofItem, 0,  lbPriv->m_nMaxWofItem, lbPriv->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbPriv->m_itemList.at ( j )->x(), lbPriv->m_itemList.at ( j )->y() +lbPriv->m_nMaxHofItem, lbPriv->m_nMaxWofItem, lbPriv->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbPriv->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbPriv->m_itemTxtFont );
        }

        lbPriv->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbPriv->m_imgFocusIn.setGeometry ( lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->x(), lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->y(), lbPriv->m_nMaxWofItem, lbPriv->m_nMaxHofItem );
        lbPriv->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbPriv->m_imgFocusOut.setGeometry ( lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->x(), lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->y(), lbPriv->m_nMaxWofItem, lbPriv->m_nMaxHofItem );
        lbPriv->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbPriv->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbPriv->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbPriv->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbPriv->m_nCurItemIndex );
        lbPriv->m_bSendHighLighted = false;
    }

    if ( lbPriv->m_bIsNeedShowItemInfo )
    {
        lbPriv->m_timerForShowItemInfo.start ( 1000, true );
        lbPriv->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbPriv->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbPriv->m_nNextItemKey )
    {
        bRetVal = moveFocusDown();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    switch ( nKey )
    {
    case Giveda::Key_Left:
        bRetVal = moveFocusLeft();
        break;
    case Giveda::Key_Right:
        bRetVal = moveFocusRight();
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlListBox::insertItem ( GCtrlListBoxItem* pItem, int index )
{
    if ( index<0 )
    {
        lbPriv->m_itemList.append ( pItem );
    }
    else
    {
        lbPriv->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbPriv->m_itemList.remove(index);
    lbPriv->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbPriv->m_nCurItemIndex;
    }

    lbPriv->m_itemList.remove ( index );
    lbPriv->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbPriv->m_nRowNums = num;
    lbPriv->m_nMaxHofItem = height() /lbPriv->m_nRowNums;
    lbPriv->m_nDrawTo = lbPriv->m_nColumnNums*lbPriv->m_nRowNums-1 + lbPriv->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbPriv->m_nColumnNums = num;
    lbPriv->m_nMaxWofItem = width() /lbPriv->m_nColumnNums;
    lbPriv->m_nDrawTo = lbPriv->m_nColumnNums*lbPriv->m_nRowNums-1 + lbPriv->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbPriv->m_nCurItemIndex;
    }
    return lbPriv->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbPriv->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbPriv->m_nDrawFrom = 0;
    lbPriv->m_nDrawTo = lbPriv->m_nColumnNums*lbPriv->m_nRowNums-1 + lbPriv->m_nDrawFrom;
    lbPriv->m_nCurItemIndex = 0;
    lbPriv->m_bIsNeedDoLayout = true;
    lbPriv->m_bIsNeedShowItemInfo = true;
    lbPriv->m_bSendHighLighted = true;

    lbPriv->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbPriv->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbPriv->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbPriv->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbPriv->m_nColumnNums*lbPriv->m_nRowNums;
    int nToScreen=lbPriv->m_nCurItemIndex/nCountsPerScreen;
    lbPriv->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbPriv->m_nDrawTo = nCountsPerScreen-1 + lbPriv->m_nDrawFrom;
    lbPriv->m_bIsNeedDoLayout = true;
    lbPriv->m_bIsNeedShowItemInfo = true;
    lbPriv->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbPriv->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbPriv->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex ) );
        if ( lbPriv->m_bScrollEnabled )
        {
            lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbPriv->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbPriv->m_bScrollEnabled )
    {
        lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbPriv->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex )->setFocus();
    lbPriv->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbPriv->m_itemList.at ( lbPriv->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbPriv->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbPriv->m_nCurItemIndex -lbPriv->m_nRowNums;
    if ( lbPriv->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbPriv->m_nCurItemIndex +lbPriv->m_nRowNums;
    if ( lbPriv->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbPriv->m_nCurItemIndex -1;
    if ( lbPriv->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbPriv->m_nCurItemIndex +1;
    if( lbPriv->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbPriv->m_nCurItemIndex;
    int nCountsPerScreen=lbPriv->m_nColumnNums*lbPriv->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbPriv->m_bIsNeedDoLayout = true;
        lbPriv->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbPriv->m_nDrawTo = lbPriv->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbPriv->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbPriv->m_itemList.at ( toIndex )->setFocus();
    if ( lbPriv->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbPriv->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbPriv->m_nCurItemIndex = toIndex;
    lbPriv->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbPriv->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbPriv->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbPriv->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbPriv->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbPriv->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbPriv->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbPriv->m_itemTxtFont;
}
void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbPriv->m_bScrollEnabled = enabled;
}
void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbPriv->m_nPreviousItemKey = nKey;
}
void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbPriv->m_nNextItemKey = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
