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

class GCtrlRadioButtonFles
{
public:
    GCtrlRadioButtonFles( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
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
     rbFhl( new GCtrlRadioButtonFles(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbFhl->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbFhl->m_pixBg.setPixmap ( pStyle->pixmap ( rbFhl->m_pixBg.name() ) );
    rbFhl->m_pixTop.setPixmap ( pStyle->pixmap ( rbFhl->m_pixTop.name() ) );
    rbFhl->m_txt.moveBy ( rbFhl->m_pixBg.width(), 0 );

    int nH = rbFhl->m_txt.getSingleLineSize().height();
    if ( rbFhl->m_pixBg.height() > nH )
    {
        nH = rbFhl->m_pixBg.height();
        rbFhl->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbFhl->m_pixBg.height() ) /2;
        rbFhl->m_pixBg.setY ( nTmp );
        rbFhl->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbFhl;
}

bool GCtrlRadioButton::isChecked()
{
    return rbFhl->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbFhl->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbFhl->m_pixBg.draw ( p );
    if ( rbFhl->m_bIsChecked )
    {
        rbFhl->m_pixTop.draw ( p );
    }
    rbFhl->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbFhl->m_bIsChecked )
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

class GCtrlRadioButtonGroupFles
{
public:
    GCtrlRadioButtonGroupFles( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
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
     rbgFhl( new GCtrlRadioButtonGroupFles(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgFhl->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgFhl->m_imgFocusIn.name() ) );
    rbgFhl->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgFhl->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgFhl;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgFhl->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgFhl->m_itemList.first(); pItem; pItem=rbgFhl->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgFhl->m_imgFocusIn.setGeometry ( rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->x(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->y(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->width(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->height() );
        rbgFhl->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgFhl->m_imgFocusOut.setGeometry ( rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->x(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->y(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->width(), rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->height() );
        rbgFhl->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex );
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
    for ( GCtrlRadioButton* pItem = rbgFhl->m_itemList.first(); pItem; pItem=rbgFhl->m_itemList.next() )
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
    rbgFhl->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgFhl->m_nCurItemIndex -1;
    if ( rbgFhl->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgFhl->m_nCurItemIndex +1;
    if ( rbgFhl->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgFhl->m_itemList.at ( toIndex )->setFocus();

    rbgFhl->m_itemList.at ( rbgFhl->m_nCurItemIndex )->update();
    rbgFhl->m_itemList.at ( toIndex )->update();

    rbgFhl->m_nCurItemIndex = toIndex;
}
GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgFhl->m_txtTitle;
}

