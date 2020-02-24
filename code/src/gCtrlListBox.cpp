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

class GCtrlListBoxMhL
{
public:
    GCtrlListBoxMhL ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
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
    lbLqH = new GCtrlListBoxMhL(frm, parent, this );
    frm->appendItem ( this );
    lbLqH->m_nDrawFrom = 0;
    lbLqH->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbLqH->m_timerForShowItemInfo,
              lbLqH->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbLqH->m_nCurItemIndex = 0;
    lbLqH->m_bIsNeedDoLayout = true;
    lbLqH->m_bIsNeedShowItemInfo = true;
    lbLqH->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbLqH->m_imgFocusIn.setImage ( pStyle->pixmap ( lbLqH->m_imgFocusIn.name() ) );
    lbLqH->m_imgFocusOut.setImage ( pStyle->pixmap ( lbLqH->m_imgFocusOut.name() ) );
    


    lbLqH->m_bScrollEnabled = true;
    lbLqH->m_nPreviousItemKey = Giveda::Key_Up;
    lbLqH->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbLqH->m_itemList.clear();
    delete lbLqH;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbLqH->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbLqH->m_itemList.count()-1<lbLqH->m_nDrawTo ? lbLqH->m_itemList.count()-1 : lbLqH->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbLqH->m_bIsNeedDoLayout )
    {
        lbLqH->m_itemList.at ( lbLqH->m_nDrawFrom )->setGeometry ( 0, 0, lbLqH->m_nMaxWofItem, lbLqH->m_nMaxHofItem );
        lbLqH->m_itemList.at ( lbLqH->m_nDrawFrom )->getTxt()->setColor ( lbLqH->m_itemTxtColor );
        lbLqH->m_itemList.at ( lbLqH->m_nDrawFrom )->getTxt()->setFont ( lbLqH->m_itemTxtFont );
        int nBottomEdge = lbLqH->m_nMaxHofItem* ( lbLqH->m_nRowNums-1 );
        for ( int j=lbLqH->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbLqH->m_itemList.at ( j+1 );
            if ( lbLqH->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbLqH->m_itemList.at ( j )->x() +lbLqH->m_nMaxWofItem, 0,  lbLqH->m_nMaxWofItem, lbLqH->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbLqH->m_itemList.at ( j )->x(), lbLqH->m_itemList.at ( j )->y() +lbLqH->m_nMaxHofItem, lbLqH->m_nMaxWofItem, lbLqH->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbLqH->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbLqH->m_itemTxtFont );
        }

        lbLqH->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbLqH->m_imgFocusIn.setGeometry ( lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->x(), lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->y(), lbLqH->m_nMaxWofItem, lbLqH->m_nMaxHofItem );
        lbLqH->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbLqH->m_imgFocusOut.setGeometry ( lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->x(), lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->y(), lbLqH->m_nMaxWofItem, lbLqH->m_nMaxHofItem );
        lbLqH->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbLqH->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbLqH->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbLqH->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbLqH->m_nCurItemIndex );
        lbLqH->m_bSendHighLighted = false;
    }

    if ( lbLqH->m_bIsNeedShowItemInfo )
    {
        lbLqH->m_timerForShowItemInfo.start ( 1000, true );
        lbLqH->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbLqH->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbLqH->m_nNextItemKey )
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
        lbLqH->m_itemList.append ( pItem );
    }
    else
    {
        lbLqH->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbLqH->m_itemList.remove(index);
    lbLqH->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbLqH->m_nCurItemIndex;
    }

    lbLqH->m_itemList.remove ( index );
    lbLqH->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbLqH->m_nRowNums = num;
    lbLqH->m_nMaxHofItem = height() /lbLqH->m_nRowNums;
    lbLqH->m_nDrawTo = lbLqH->m_nColumnNums*lbLqH->m_nRowNums-1 + lbLqH->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbLqH->m_nColumnNums = num;
    lbLqH->m_nMaxWofItem = width() /lbLqH->m_nColumnNums;
    lbLqH->m_nDrawTo = lbLqH->m_nColumnNums*lbLqH->m_nRowNums-1 + lbLqH->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbLqH->m_nCurItemIndex;
    }
    return lbLqH->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbLqH->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbLqH->m_nDrawFrom = 0;
    lbLqH->m_nDrawTo = lbLqH->m_nColumnNums*lbLqH->m_nRowNums-1 + lbLqH->m_nDrawFrom;
    lbLqH->m_nCurItemIndex = 0;
    lbLqH->m_bIsNeedDoLayout = true;
    lbLqH->m_bIsNeedShowItemInfo = true;
    lbLqH->m_bSendHighLighted = true;

    lbLqH->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbLqH->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbLqH->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbLqH->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbLqH->m_nColumnNums*lbLqH->m_nRowNums;
    int nToScreen=lbLqH->m_nCurItemIndex/nCountsPerScreen;
    lbLqH->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbLqH->m_nDrawTo = nCountsPerScreen-1 + lbLqH->m_nDrawFrom;
    lbLqH->m_bIsNeedDoLayout = true;
    lbLqH->m_bIsNeedShowItemInfo = true;
    lbLqH->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbLqH->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbLqH->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex ) );
        if ( lbLqH->m_bScrollEnabled )
        {
            lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbLqH->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbLqH->m_bScrollEnabled )
    {
        lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbLqH->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex )->setFocus();
    lbLqH->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbLqH->m_itemList.at ( lbLqH->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbLqH->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbLqH->m_nCurItemIndex -lbLqH->m_nRowNums;
    if ( lbLqH->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbLqH->m_nCurItemIndex +lbLqH->m_nRowNums;
    if ( lbLqH->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbLqH->m_nCurItemIndex -1;
    if ( lbLqH->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbLqH->m_nCurItemIndex +1;
    if( lbLqH->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbLqH->m_nCurItemIndex;
    int nCountsPerScreen=lbLqH->m_nColumnNums*lbLqH->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbLqH->m_bIsNeedDoLayout = true;
        lbLqH->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbLqH->m_nDrawTo = lbLqH->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbLqH->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbLqH->m_itemList.at ( toIndex )->setFocus();
    if ( lbLqH->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbLqH->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbLqH->m_nCurItemIndex = toIndex;
    lbLqH->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbLqH->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbLqH->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbLqH->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbLqH->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbLqH->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbLqH->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbLqH->m_itemTxtFont;
}

void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbLqH->m_bScrollEnabled = enabled;
}

void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbLqH->m_nPreviousItemKey = nKey;
}

void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbLqH->m_nNextItemKey = nKey;
}

