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
#include "gCtrlRadioButtonGroup.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlRadioButtonHmq
{
public:
    GCtrlRadioButtonHmq( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
        :     m_pixBg ( frm, parent, "radioButtonUnchecked" ),
              m_pixTop ( frm, parent, "radioButtonChecked" ),
              m_txt ( str, frm, parent, "radioButtonTxt" ),
              m_bIsChecked ( check )
    {
        
    }
    
    GMPixmap m_pixBg;
    GMPixmap m_pixTop;
    GMText m_txt;
    bool m_bIsChecked;
};

GCtrlRadioButton::GCtrlRadioButton ( const GString& str, GCtrlForm* frm, GCtrlRadioButtonGroup* parent, const char* name, bool check )
    :GMCtrlItem ( frm, parent, name ),
     rbMqs( new GCtrlRadioButtonHmq(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbMqs->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbMqs->m_pixBg.setPixmap ( pStyle->pixmap ( rbMqs->m_pixBg.name() ) );
    rbMqs->m_pixTop.setPixmap ( pStyle->pixmap ( rbMqs->m_pixTop.name() ) );
    rbMqs->m_txt.moveBy ( rbMqs->m_pixBg.width(), 0 );

    int nH = rbMqs->m_txt.getSingleLineSize().height();
    if ( rbMqs->m_pixBg.height() > nH )
    {
        nH = rbMqs->m_pixBg.height();
        rbMqs->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbMqs->m_pixBg.height() ) /2;
        rbMqs->m_pixBg.setY ( nTmp );
        rbMqs->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbMqs;
}

bool GCtrlRadioButton::isChecked()
{
    return rbMqs->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbMqs->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbMqs->m_pixBg.draw ( p );
    if ( rbMqs->m_bIsChecked )
    {
        rbMqs->m_pixTop.draw ( p );
    }
    rbMqs->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbMqs->m_bIsChecked )
        {
            GCtrlRadioButtonGroup* pBtnGroup = ( GCtrlRadioButtonGroup* ) parent();
            pBtnGroup->uncheckedOther ( this );
            setChecked ( true );
            emitCheckedChanged();
        }
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlRadioButton::emitCheckedChanged()
{
    checkedChanged.emit();
    update();
}

class GCtrlRadioButtonGroupHmq
{
public:
    GCtrlRadioButtonGroupHmq( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
        :     m_imgFocusIn ( frm, parent, "radioButtonGroupFocusIn" ),
              m_imgFocusOut ( frm, parent, "radioButtonGroupFocusOut" ),
              m_txtTitle ( title, frm, parent, "radioButtonGroupTitle" ),
              m_nCurItemIndex ( 0 )
    {}
    GPtrList<GCtrlRadioButton> m_itemList;
    GMImage m_imgFocusIn;
    GMImage m_imgFocusOut;
    GMText m_txtTitle;
    int m_nCurItemIndex;
};

GCtrlRadioButtonGroup::GCtrlRadioButtonGroup ( const GString& title, GCtrlForm* frm, GMItem* parent, const char* name )
    :GMContainerItem ( frm, parent, name ),
     rbgMqs( new GCtrlRadioButtonGroupHmq(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgMqs->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgMqs->m_imgFocusIn.name() ) );
    rbgMqs->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgMqs->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgMqs;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgMqs->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgMqs->m_itemList.first(); pItem; pItem=rbgMqs->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgMqs->m_imgFocusIn.setGeometry ( rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->x(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->y(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->width(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->height() );
        rbgMqs->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgMqs->m_imgFocusOut.setGeometry ( rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->x(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->y(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->width(), rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->height() );
        rbgMqs->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Up:
        bRetVal = moveFocusUp();
        break;
    case Giveda::Key_Down:
        bRetVal = moveFocusDown();
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlRadioButtonGroup::uncheckedOther ( GCtrlRadioButton* pBtn )
{
    for ( GCtrlRadioButton* pItem = rbgMqs->m_itemList.first(); pItem; pItem=rbgMqs->m_itemList.next() )
    {
        if ( pBtn != pItem && pItem->isChecked() )
        {
            pItem->setChecked ( false );
            pItem->emitCheckedChanged();
        }
    }
}

void GCtrlRadioButtonGroup::appendItem ( GCtrlRadioButton* pItem )
{
    rbgMqs->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgMqs->m_nCurItemIndex -1;
    if ( rbgMqs->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgMqs->m_nCurItemIndex +1;
    if ( rbgMqs->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgMqs->m_itemList.at ( toIndex )->setFocus();

    rbgMqs->m_itemList.at ( rbgMqs->m_nCurItemIndex )->update();
    rbgMqs->m_itemList.at ( toIndex )->update();

    rbgMqs->m_nCurItemIndex = toIndex;
}

GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgMqs->m_txtTitle;
}

// reborn, a nice day ^_^
