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

class GCtrlListBoxFles
{
public:
    GCtrlListBoxFles ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
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
    lbFhl = new GCtrlListBoxFles(frm, parent, this );
    frm->appendItem ( this );
    lbFhl->m_nDrawFrom = 0;
    lbFhl->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbFhl->m_timerForShowItemInfo,
              lbFhl->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbFhl->m_nCurItemIndex = 0;
    lbFhl->m_bIsNeedDoLayout = true;
    lbFhl->m_bIsNeedShowItemInfo = true;
    lbFhl->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbFhl->m_imgFocusIn.setImage ( pStyle->pixmap ( lbFhl->m_imgFocusIn.name() ) );
    lbFhl->m_imgFocusOut.setImage ( pStyle->pixmap ( lbFhl->m_imgFocusOut.name() ) );
    
    lbFhl->m_itemList.setAutoDelete ( true );

    lbFhl->m_bScrollEnabled = true;
    lbFhl->m_nPreviousItemKey = Giveda::Key_Up;
    lbFhl->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbFhl->m_itemList.clear();
    delete lbFhl;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbFhl->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbFhl->m_itemList.count()-1<lbFhl->m_nDrawTo ? lbFhl->m_itemList.count()-1 : lbFhl->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbFhl->m_bIsNeedDoLayout )
    {
        lbFhl->m_itemList.at ( lbFhl->m_nDrawFrom )->setGeometry ( 0, 0, lbFhl->m_nMaxWofItem, lbFhl->m_nMaxHofItem );
        lbFhl->m_itemList.at ( lbFhl->m_nDrawFrom )->getTxt()->setColor ( lbFhl->m_itemTxtColor );
        lbFhl->m_itemList.at ( lbFhl->m_nDrawFrom )->getTxt()->setFont ( lbFhl->m_itemTxtFont );
        int nBottomEdge = lbFhl->m_nMaxHofItem* ( lbFhl->m_nRowNums-1 );
        for ( int j=lbFhl->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbFhl->m_itemList.at ( j+1 );
            if ( lbFhl->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbFhl->m_itemList.at ( j )->x() +lbFhl->m_nMaxWofItem, 0,  lbFhl->m_nMaxWofItem, lbFhl->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbFhl->m_itemList.at ( j )->x(), lbFhl->m_itemList.at ( j )->y() +lbFhl->m_nMaxHofItem, lbFhl->m_nMaxWofItem, lbFhl->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbFhl->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbFhl->m_itemTxtFont );
        }

        lbFhl->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbFhl->m_imgFocusIn.setGeometry ( lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->x(), lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->y(), lbFhl->m_nMaxWofItem, lbFhl->m_nMaxHofItem );
        lbFhl->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbFhl->m_imgFocusOut.setGeometry ( lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->x(), lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->y(), lbFhl->m_nMaxWofItem, lbFhl->m_nMaxHofItem );
        lbFhl->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbFhl->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbFhl->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbFhl->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbFhl->m_nCurItemIndex );
        lbFhl->m_bSendHighLighted = false;
    }

    if ( lbFhl->m_bIsNeedShowItemInfo )
    {
        lbFhl->m_timerForShowItemInfo.start ( 1000, true );
        lbFhl->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbFhl->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbFhl->m_nNextItemKey )
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
        lbFhl->m_itemList.append ( pItem );
    }
    else
    {
        lbFhl->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbFhl->m_itemList.remove(index);
    lbFhl->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbFhl->m_nCurItemIndex;
    }

    lbFhl->m_itemList.remove ( index );
    lbFhl->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbFhl->m_nRowNums = num;
    lbFhl->m_nMaxHofItem = height() /lbFhl->m_nRowNums;
    lbFhl->m_nDrawTo = lbFhl->m_nColumnNums*lbFhl->m_nRowNums-1 + lbFhl->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbFhl->m_nColumnNums = num;
    lbFhl->m_nMaxWofItem = width() /lbFhl->m_nColumnNums;
    lbFhl->m_nDrawTo = lbFhl->m_nColumnNums*lbFhl->m_nRowNums-1 + lbFhl->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbFhl->m_nCurItemIndex;
    }
    return lbFhl->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbFhl->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbFhl->m_nDrawFrom = 0;
    lbFhl->m_nDrawTo = lbFhl->m_nColumnNums*lbFhl->m_nRowNums-1 + lbFhl->m_nDrawFrom;
    lbFhl->m_nCurItemIndex = 0;
    lbFhl->m_bIsNeedDoLayout = true;
    lbFhl->m_bIsNeedShowItemInfo = true;
    lbFhl->m_bSendHighLighted = true;

    lbFhl->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbFhl->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbFhl->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbFhl->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbFhl->m_nColumnNums*lbFhl->m_nRowNums;
    int nToScreen=lbFhl->m_nCurItemIndex/nCountsPerScreen;
    lbFhl->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbFhl->m_nDrawTo = nCountsPerScreen-1 + lbFhl->m_nDrawFrom;
    lbFhl->m_bIsNeedDoLayout = true;
    lbFhl->m_bIsNeedShowItemInfo = true;
    lbFhl->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbFhl->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbFhl->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex ) );
        if ( lbFhl->m_bScrollEnabled )
        {
            lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbFhl->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbFhl->m_bScrollEnabled )
    {
        lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbFhl->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex )->setFocus();
    lbFhl->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbFhl->m_itemList.at ( lbFhl->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbFhl->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbFhl->m_nCurItemIndex -lbFhl->m_nRowNums;
    if ( lbFhl->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbFhl->m_nCurItemIndex +lbFhl->m_nRowNums;
    if ( lbFhl->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbFhl->m_nCurItemIndex -1;
    if ( lbFhl->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbFhl->m_nCurItemIndex +1;
    if( lbFhl->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbFhl->m_nCurItemIndex;
    int nCountsPerScreen=lbFhl->m_nColumnNums*lbFhl->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbFhl->m_bIsNeedDoLayout = true;
        lbFhl->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbFhl->m_nDrawTo = lbFhl->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbFhl->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbFhl->m_itemList.at ( toIndex )->setFocus();
    if ( lbFhl->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbFhl->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbFhl->m_nCurItemIndex = toIndex;
    lbFhl->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbFhl->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbFhl->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbFhl->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbFhl->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbFhl->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbFhl->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbFhl->m_itemTxtFont;
}

void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbFhl->m_bScrollEnabled = enabled;
}

void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbFhl->m_nPreviousItemKey = nKey;
}

void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbFhl->m_nNextItemKey = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
