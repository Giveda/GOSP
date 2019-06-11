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

class GCtrlRadioButtonPrivate
{
public:
    GCtrlRadioButtonPrivate( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
        :     m_pixBg ( frm, parent, "radioButtonUnchecked" ),
              m_pixTop ( frm, parent, "radioButtonChecked" ),
              m_txt ( str, frm, parent, "radioButtonTxt" ),
              m_bIsChecked ( check )
    {}
    GMPixmap m_pixBg;
    GMPixmap m_pixTop;
    GMText m_txt;
    bool m_bIsChecked;
};

GCtrlRadioButton::GCtrlRadioButton ( const GString& str, GCtrlForm* frm, GCtrlRadioButtonGroup* parent, const char* name, bool check )
    :GMCtrlItem ( frm, parent, name ),
     rbPriv( new GCtrlRadioButtonPrivate(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbPriv->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbPriv->m_pixBg.setPixmap ( pStyle->pixmap ( rbPriv->m_pixBg.name() ) );
    rbPriv->m_pixTop.setPixmap ( pStyle->pixmap ( rbPriv->m_pixTop.name() ) );
    rbPriv->m_txt.moveBy ( rbPriv->m_pixBg.width(), 0 );

    int nH = rbPriv->m_txt.getSingleLineSize().height();
    if ( rbPriv->m_pixBg.height() > nH )
    {
        nH = rbPriv->m_pixBg.height();
        rbPriv->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbPriv->m_pixBg.height() ) /2;
        rbPriv->m_pixBg.setY ( nTmp );
        rbPriv->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbPriv;
}

bool GCtrlRadioButton::isChecked()
{
    return rbPriv->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbPriv->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbPriv->m_pixBg.draw ( p );
    if ( rbPriv->m_bIsChecked )
    {
        rbPriv->m_pixTop.draw ( p );
    }
    rbPriv->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbPriv->m_bIsChecked )
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

class GCtrlRadioButtonGroupPrivate
{
public:
    GCtrlRadioButtonGroupPrivate( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
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
     rbgPriv( new GCtrlRadioButtonGroupPrivate(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgPriv->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgPriv->m_imgFocusIn.name() ) );
    rbgPriv->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgPriv->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgPriv;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgPriv->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgPriv->m_itemList.first(); pItem; pItem=rbgPriv->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgPriv->m_imgFocusIn.setGeometry ( rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->x(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->y(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->width(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->height() );
        rbgPriv->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgPriv->m_imgFocusOut.setGeometry ( rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->x(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->y(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->width(), rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->height() );
        rbgPriv->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex );
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
    for ( GCtrlRadioButton* pItem = rbgPriv->m_itemList.first(); pItem; pItem=rbgPriv->m_itemList.next() )
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
    rbgPriv->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgPriv->m_nCurItemIndex -1;
    if ( rbgPriv->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgPriv->m_nCurItemIndex +1;
    if ( rbgPriv->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgPriv->m_itemList.at ( toIndex )->setFocus();

    rbgPriv->m_itemList.at ( rbgPriv->m_nCurItemIndex )->update();
    rbgPriv->m_itemList.at ( toIndex )->update();

    rbgPriv->m_nCurItemIndex = toIndex;
}
GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgPriv->m_txtTitle;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
