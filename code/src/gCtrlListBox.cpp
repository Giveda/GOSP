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
    :GMCtrlItem ( frm, parent, name ), 
    m_txt ( str, frm, this, "listBoxText" )
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

class GCtrlListBoxHmq
{
public:
    GCtrlListBoxHmq ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
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
    lbMqs = new GCtrlListBoxHmq(frm, parent, this );
    frm->appendItem ( this );
    lbMqs->m_nDrawFrom = 0;
    lbMqs->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbMqs->m_timerForShowItemInfo,
              lbMqs->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbMqs->m_nCurItemIndex = 0;
    lbMqs->m_bIsNeedDoLayout = true;
    lbMqs->m_bIsNeedShowItemInfo = true;
    lbMqs->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbMqs->m_imgFocusIn.setImage ( pStyle->pixmap ( lbMqs->m_imgFocusIn.name() ) );
    lbMqs->m_imgFocusOut.setImage ( pStyle->pixmap ( lbMqs->m_imgFocusOut.name() ) );
    
    lbMqs->m_itemList.setAutoDelete ( true );

    lbMqs->m_bScrollEnabled = true;
    lbMqs->m_nPreviousItemKey = Giveda::Key_Up;
    lbMqs->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbMqs->m_itemList.clear();
    delete lbMqs;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbMqs->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbMqs->m_itemList.count()-1<lbMqs->m_nDrawTo ? lbMqs->m_itemList.count()-1 : lbMqs->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbMqs->m_bIsNeedDoLayout )
    {
        lbMqs->m_itemList.at ( lbMqs->m_nDrawFrom )->setGeometry ( 0, 0, lbMqs->m_nMaxWofItem, lbMqs->m_nMaxHofItem );
        lbMqs->m_itemList.at ( lbMqs->m_nDrawFrom )->getTxt()->setColor ( lbMqs->m_itemTxtColor );
        lbMqs->m_itemList.at ( lbMqs->m_nDrawFrom )->getTxt()->setFont ( lbMqs->m_itemTxtFont );
        int nBottomEdge = lbMqs->m_nMaxHofItem* ( lbMqs->m_nRowNums-1 );
        for ( int j=lbMqs->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbMqs->m_itemList.at ( j+1 );
            if ( lbMqs->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbMqs->m_itemList.at ( j )->x() +lbMqs->m_nMaxWofItem, 0,  lbMqs->m_nMaxWofItem, lbMqs->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbMqs->m_itemList.at ( j )->x(), lbMqs->m_itemList.at ( j )->y() +lbMqs->m_nMaxHofItem, lbMqs->m_nMaxWofItem, lbMqs->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbMqs->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbMqs->m_itemTxtFont );
        }

        lbMqs->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbMqs->m_imgFocusIn.setGeometry ( lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->x(), lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->y(), lbMqs->m_nMaxWofItem, lbMqs->m_nMaxHofItem );
        lbMqs->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbMqs->m_imgFocusOut.setGeometry ( lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->x(), lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->y(), lbMqs->m_nMaxWofItem, lbMqs->m_nMaxHofItem );
        lbMqs->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbMqs->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbMqs->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbMqs->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbMqs->m_nCurItemIndex );
        lbMqs->m_bSendHighLighted = false;
    }

    if ( lbMqs->m_bIsNeedShowItemInfo )
    {
        lbMqs->m_timerForShowItemInfo.start ( 1000, true );
        lbMqs->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbMqs->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbMqs->m_nNextItemKey )
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
        lbMqs->m_itemList.append ( pItem );
    }
    else
    {
        lbMqs->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbMqs->m_itemList.remove(index);
    lbMqs->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbMqs->m_nCurItemIndex;
    }

    lbMqs->m_itemList.remove ( index );
    lbMqs->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbMqs->m_nRowNums = num;
    lbMqs->m_nMaxHofItem = height() /lbMqs->m_nRowNums;
    lbMqs->m_nDrawTo = lbMqs->m_nColumnNums*lbMqs->m_nRowNums-1 + lbMqs->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbMqs->m_nColumnNums = num;
    lbMqs->m_nMaxWofItem = width() /lbMqs->m_nColumnNums;
    lbMqs->m_nDrawTo = lbMqs->m_nColumnNums*lbMqs->m_nRowNums-1 + lbMqs->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbMqs->m_nCurItemIndex;
    }
    return lbMqs->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbMqs->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbMqs->m_nDrawFrom = 0;
    lbMqs->m_nDrawTo = lbMqs->m_nColumnNums*lbMqs->m_nRowNums-1 + lbMqs->m_nDrawFrom;
    lbMqs->m_nCurItemIndex = 0;
    lbMqs->m_bIsNeedDoLayout = true;
    lbMqs->m_bIsNeedShowItemInfo = true;
    lbMqs->m_bSendHighLighted = true;

    lbMqs->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbMqs->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbMqs->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbMqs->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbMqs->m_nColumnNums*lbMqs->m_nRowNums;
    int nToScreen=lbMqs->m_nCurItemIndex/nCountsPerScreen;
    lbMqs->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbMqs->m_nDrawTo = nCountsPerScreen-1 + lbMqs->m_nDrawFrom;
    lbMqs->m_bIsNeedDoLayout = true;
    lbMqs->m_bIsNeedShowItemInfo = true;
    lbMqs->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbMqs->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbMqs->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex ) );
        if ( lbMqs->m_bScrollEnabled )
        {
            lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbMqs->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbMqs->m_bScrollEnabled )
    {
        lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbMqs->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex )->setFocus();
    lbMqs->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbMqs->m_itemList.at ( lbMqs->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbMqs->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbMqs->m_nCurItemIndex -lbMqs->m_nRowNums;
    if ( lbMqs->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbMqs->m_nCurItemIndex +lbMqs->m_nRowNums;
    if ( lbMqs->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbMqs->m_nCurItemIndex -1;
    if ( lbMqs->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbMqs->m_nCurItemIndex +1;
    if( lbMqs->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbMqs->m_nCurItemIndex;
    int nCountsPerScreen=lbMqs->m_nColumnNums*lbMqs->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbMqs->m_bIsNeedDoLayout = true;
        lbMqs->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbMqs->m_nDrawTo = lbMqs->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbMqs->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbMqs->m_itemList.at ( toIndex )->setFocus();
    if ( lbMqs->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbMqs->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbMqs->m_nCurItemIndex = toIndex;
    lbMqs->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbMqs->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbMqs->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbMqs->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbMqs->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbMqs->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbMqs->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbMqs->m_itemTxtFont;
}

void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbMqs->m_bScrollEnabled = enabled;
}

void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbMqs->m_nPreviousItemKey = nKey;
}

void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbMqs->m_nNextItemKey = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
