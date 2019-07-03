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

class GCtrlRadioButtonSelf
{
public:
    GCtrlRadioButtonSelf( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
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
     rbSpp( new GCtrlRadioButtonSelf(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbSpp->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbSpp->m_pixBg.setPixmap ( pStyle->pixmap ( rbSpp->m_pixBg.name() ) );
    rbSpp->m_pixTop.setPixmap ( pStyle->pixmap ( rbSpp->m_pixTop.name() ) );
    rbSpp->m_txt.moveBy ( rbSpp->m_pixBg.width(), 0 );

    int nH = rbSpp->m_txt.getSingleLineSize().height();
    if ( rbSpp->m_pixBg.height() > nH )
    {
        nH = rbSpp->m_pixBg.height();
        rbSpp->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbSpp->m_pixBg.height() ) /2;
        rbSpp->m_pixBg.setY ( nTmp );
        rbSpp->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbSpp;
}

bool GCtrlRadioButton::isChecked()
{
    return rbSpp->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbSpp->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbSpp->m_pixBg.draw ( p );
    if ( rbSpp->m_bIsChecked )
    {
        rbSpp->m_pixTop.draw ( p );
    }
    rbSpp->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbSpp->m_bIsChecked )
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

class GCtrlRadioButtonGroupSelf
{
public:
    GCtrlRadioButtonGroupSelf( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
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
     rbgSpp( new GCtrlRadioButtonGroupSelf(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgSpp->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgSpp->m_imgFocusIn.name() ) );
    rbgSpp->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgSpp->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgSpp;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgSpp->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgSpp->m_itemList.first(); pItem; pItem=rbgSpp->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgSpp->m_imgFocusIn.setGeometry ( rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->x(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->y(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->width(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->height() );
        rbgSpp->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgSpp->m_imgFocusOut.setGeometry ( rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->x(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->y(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->width(), rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->height() );
        rbgSpp->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex );
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
    for ( GCtrlRadioButton* pItem = rbgSpp->m_itemList.first(); pItem; pItem=rbgSpp->m_itemList.next() )
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
    rbgSpp->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgSpp->m_nCurItemIndex -1;
    if ( rbgSpp->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgSpp->m_nCurItemIndex +1;
    if ( rbgSpp->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgSpp->m_itemList.at ( toIndex )->setFocus();

    rbgSpp->m_itemList.at ( rbgSpp->m_nCurItemIndex )->update();
    rbgSpp->m_itemList.at ( toIndex )->update();

    rbgSpp->m_nCurItemIndex = toIndex;
}

GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgSpp->m_txtTitle;
}

// have a nice day ^_^
