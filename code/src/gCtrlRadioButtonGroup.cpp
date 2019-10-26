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

class GCtrlRadioButtonMhL
{
public:
    GCtrlRadioButtonMhL( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
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
     rbLqH( new GCtrlRadioButtonMhL(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbLqH->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbLqH->m_pixBg.setPixmap ( pStyle->pixmap ( rbLqH->m_pixBg.name() ) );
    rbLqH->m_pixTop.setPixmap ( pStyle->pixmap ( rbLqH->m_pixTop.name() ) );
    rbLqH->m_txt.moveBy ( rbLqH->m_pixBg.width(), 0 );

    int nH = rbLqH->m_txt.getSingleLineSize().height();
    if ( rbLqH->m_pixBg.height() > nH )
    {
        nH = rbLqH->m_pixBg.height();
        rbLqH->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbLqH->m_pixBg.height() ) /2;
        rbLqH->m_pixBg.setY ( nTmp );
        rbLqH->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbLqH;
}

bool GCtrlRadioButton::isChecked()
{
    return rbLqH->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbLqH->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbLqH->m_pixBg.draw ( p );
    if ( rbLqH->m_bIsChecked )
    {
        rbLqH->m_pixTop.draw ( p );
    }
    rbLqH->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbLqH->m_bIsChecked )
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

class GCtrlRadioButtonGroupMhL
{
public:
    GCtrlRadioButtonGroupMhL( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
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
     rbgLqH( new GCtrlRadioButtonGroupMhL(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgLqH->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgLqH->m_imgFocusIn.name() ) );
    rbgLqH->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgLqH->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgLqH;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgLqH->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgLqH->m_itemList.first(); pItem; pItem=rbgLqH->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgLqH->m_imgFocusIn.setGeometry ( rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->x(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->y(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->width(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->height() );
        rbgLqH->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgLqH->m_imgFocusOut.setGeometry ( rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->x(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->y(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->width(), rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->height() );
        rbgLqH->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex );
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
    for ( GCtrlRadioButton* pItem = rbgLqH->m_itemList.first(); pItem; pItem=rbgLqH->m_itemList.next() )
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
    rbgLqH->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgLqH->m_nCurItemIndex -1;
    if ( rbgLqH->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgLqH->m_nCurItemIndex +1;
    if ( rbgLqH->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgLqH->m_itemList.at ( toIndex )->setFocus();

    rbgLqH->m_itemList.at ( rbgLqH->m_nCurItemIndex )->update();
    rbgLqH->m_itemList.at ( toIndex )->update();

    rbgLqH->m_nCurItemIndex = toIndex;
}

GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgLqH->m_txtTitle;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
