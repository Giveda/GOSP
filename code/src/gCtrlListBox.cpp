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

class GCtrlListBoxLMQ
{
public:
    GCtrlListBoxLMQ ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
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
    lbHlo = new GCtrlListBoxLMQ(frm, parent, this );
    frm->appendItem ( this );
    lbHlo->m_nDrawFrom = 0;
    lbHlo->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbHlo->m_timerForShowItemInfo,
              lbHlo->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbHlo->m_nCurItemIndex = 0;
    lbHlo->m_bIsNeedDoLayout = true;
    lbHlo->m_bIsNeedShowItemInfo = true;
    lbHlo->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbHlo->m_imgFocusIn.setImage ( pStyle->pixmap ( lbHlo->m_imgFocusIn.name() ) );
    lbHlo->m_imgFocusOut.setImage ( pStyle->pixmap ( lbHlo->m_imgFocusOut.name() ) );
    
    lbHlo->m_itemList.setAutoDelete ( true );

    lbHlo->m_bScrollEnabled = true;
    lbHlo->m_nPreviousItemKey = Giveda::Key_Up;
    lbHlo->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbHlo->m_itemList.clear();
    delete lbHlo;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbHlo->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbHlo->m_itemList.count()-1<lbHlo->m_nDrawTo ? lbHlo->m_itemList.count()-1 : lbHlo->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbHlo->m_bIsNeedDoLayout )
    {
        lbHlo->m_itemList.at ( lbHlo->m_nDrawFrom )->setGeometry ( 0, 0, lbHlo->m_nMaxWofItem, lbHlo->m_nMaxHofItem );
        lbHlo->m_itemList.at ( lbHlo->m_nDrawFrom )->getTxt()->setColor ( lbHlo->m_itemTxtColor );
        lbHlo->m_itemList.at ( lbHlo->m_nDrawFrom )->getTxt()->setFont ( lbHlo->m_itemTxtFont );
        int nBottomEdge = lbHlo->m_nMaxHofItem* ( lbHlo->m_nRowNums-1 );
        for ( int j=lbHlo->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbHlo->m_itemList.at ( j+1 );
            if ( lbHlo->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbHlo->m_itemList.at ( j )->x() +lbHlo->m_nMaxWofItem, 0,  lbHlo->m_nMaxWofItem, lbHlo->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbHlo->m_itemList.at ( j )->x(), lbHlo->m_itemList.at ( j )->y() +lbHlo->m_nMaxHofItem, lbHlo->m_nMaxWofItem, lbHlo->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbHlo->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbHlo->m_itemTxtFont );
        }

        lbHlo->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbHlo->m_imgFocusIn.setGeometry ( lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->x(), lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->y(), lbHlo->m_nMaxWofItem, lbHlo->m_nMaxHofItem );
        lbHlo->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbHlo->m_imgFocusOut.setGeometry ( lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->x(), lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->y(), lbHlo->m_nMaxWofItem, lbHlo->m_nMaxHofItem );
        lbHlo->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbHlo->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbHlo->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbHlo->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbHlo->m_nCurItemIndex );
        lbHlo->m_bSendHighLighted = false;
    }

    if ( lbHlo->m_bIsNeedShowItemInfo )
    {
        lbHlo->m_timerForShowItemInfo.start ( 1000, true );
        lbHlo->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbHlo->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbHlo->m_nNextItemKey )
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
        lbHlo->m_itemList.append ( pItem );
    }
    else
    {
        lbHlo->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbHlo->m_itemList.remove(index);
    lbHlo->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbHlo->m_nCurItemIndex;
    }

    lbHlo->m_itemList.remove ( index );
    lbHlo->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbHlo->m_nRowNums = num;
    lbHlo->m_nMaxHofItem = height() /lbHlo->m_nRowNums;
    lbHlo->m_nDrawTo = lbHlo->m_nColumnNums*lbHlo->m_nRowNums-1 + lbHlo->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbHlo->m_nColumnNums = num;
    lbHlo->m_nMaxWofItem = width() /lbHlo->m_nColumnNums;
    lbHlo->m_nDrawTo = lbHlo->m_nColumnNums*lbHlo->m_nRowNums-1 + lbHlo->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbHlo->m_nCurItemIndex;
    }
    return lbHlo->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbHlo->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbHlo->m_nDrawFrom = 0;
    lbHlo->m_nDrawTo = lbHlo->m_nColumnNums*lbHlo->m_nRowNums-1 + lbHlo->m_nDrawFrom;
    lbHlo->m_nCurItemIndex = 0;
    lbHlo->m_bIsNeedDoLayout = true;
    lbHlo->m_bIsNeedShowItemInfo = true;
    lbHlo->m_bSendHighLighted = true;

    lbHlo->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbHlo->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbHlo->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbHlo->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbHlo->m_nColumnNums*lbHlo->m_nRowNums;
    int nToScreen=lbHlo->m_nCurItemIndex/nCountsPerScreen;
    lbHlo->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbHlo->m_nDrawTo = nCountsPerScreen-1 + lbHlo->m_nDrawFrom;
    lbHlo->m_bIsNeedDoLayout = true;
    lbHlo->m_bIsNeedShowItemInfo = true;
    lbHlo->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbHlo->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbHlo->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex ) );
        if ( lbHlo->m_bScrollEnabled )
        {
            lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbHlo->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbHlo->m_bScrollEnabled )
    {
        lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbHlo->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex )->setFocus();
    lbHlo->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbHlo->m_itemList.at ( lbHlo->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbHlo->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbHlo->m_nCurItemIndex -lbHlo->m_nRowNums;
    if ( lbHlo->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbHlo->m_nCurItemIndex +lbHlo->m_nRowNums;
    if ( lbHlo->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbHlo->m_nCurItemIndex -1;
    if ( lbHlo->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbHlo->m_nCurItemIndex +1;
    if( lbHlo->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbHlo->m_nCurItemIndex;
    int nCountsPerScreen=lbHlo->m_nColumnNums*lbHlo->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbHlo->m_bIsNeedDoLayout = true;
        lbHlo->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbHlo->m_nDrawTo = lbHlo->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbHlo->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbHlo->m_itemList.at ( toIndex )->setFocus();
    if ( lbHlo->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbHlo->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbHlo->m_nCurItemIndex = toIndex;
    lbHlo->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbHlo->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbHlo->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbHlo->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbHlo->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbHlo->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbHlo->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbHlo->m_itemTxtFont;
}

void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbHlo->m_bScrollEnabled = enabled;
}

void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbHlo->m_nPreviousItemKey = nKey;
}

void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbHlo->m_nNextItemKey = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
