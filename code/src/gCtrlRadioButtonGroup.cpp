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

class GCtrlRadioButtonLMQ
{
public:
    GCtrlRadioButtonLMQ( const GString& str, GCtrlForm* frm, GCtrlRadioButton* parent, bool check)
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
     rbHlo( new GCtrlRadioButtonLMQ(str, frm, this, check) )
{
    parent->appendItem ( this );
    rbHlo->m_txt.setTextFlags ( Giveda::AlignVCenter );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonStyle();
    }
    rbHlo->m_pixBg.setPixmap ( pStyle->pixmap ( rbHlo->m_pixBg.name() ) );
    rbHlo->m_pixTop.setPixmap ( pStyle->pixmap ( rbHlo->m_pixTop.name() ) );
    rbHlo->m_txt.moveBy ( rbHlo->m_pixBg.width(), 0 );

    int nH = rbHlo->m_txt.getSingleLineSize().height();
    if ( rbHlo->m_pixBg.height() > nH )
    {
        nH = rbHlo->m_pixBg.height();
        rbHlo->m_txt.setHeight(nH);
    }
    else
    {
        int nTmp = ( nH-rbHlo->m_pixBg.height() ) /2;
        rbHlo->m_pixBg.setY ( nTmp );
        rbHlo->m_pixTop.setY ( nTmp );
    }
    setHeight ( nH );
}

GCtrlRadioButton::~GCtrlRadioButton()
{
    delete rbHlo;
}

bool GCtrlRadioButton::isChecked()
{
    return rbHlo->m_bIsChecked;
}

void GCtrlRadioButton::setChecked ( bool b )
{
    rbHlo->m_bIsChecked=b;
}

void GCtrlRadioButton::paintEvent ( GPainter& p )
{
    rbHlo->m_pixBg.draw ( p );
    if ( rbHlo->m_bIsChecked )
    {
        rbHlo->m_pixTop.draw ( p );
    }
    rbHlo->m_txt.draw ( p );
}

bool GCtrlRadioButton::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        if ( !rbHlo->m_bIsChecked )
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

class GCtrlRadioButtonGroupLMQ
{
public:
    GCtrlRadioButtonGroupLMQ( const GString& title, GCtrlForm* frm, GCtrlRadioButtonGroup* parent)
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
     rbgHlo( new GCtrlRadioButtonGroupLMQ(title, frm, this) )
{
    frm->appendItem ( this );
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendRadioButtonGroupStyle();
    }
    rbgHlo->m_imgFocusIn.setImage ( pStyle->pixmap ( rbgHlo->m_imgFocusIn.name() ) );
    rbgHlo->m_imgFocusOut.setImage ( pStyle->pixmap ( rbgHlo->m_imgFocusOut.name() ) );
}

GCtrlRadioButtonGroup::~GCtrlRadioButtonGroup()
{
    delete rbgHlo;
}

void GCtrlRadioButtonGroup::paintEvent ( GPainter& p )
{
    rbgHlo->m_txtTitle.draw ( p );

    for ( GCtrlRadioButton* pItem = rbgHlo->m_itemList.first(); pItem; pItem=rbgHlo->m_itemList.next() )
    {
        pItem->setWidth ( width()-pItem->x() );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( hasFocus() )
    {
        rbgHlo->m_imgFocusIn.setGeometry ( rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->x(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->y(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->width(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->height() );
        rbgHlo->m_imgFocusIn.draw ( p );
    }
    else
    {
        rbgHlo->m_imgFocusOut.setGeometry ( rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->x(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->y(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->width(), rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->height() );
        rbgHlo->m_imgFocusOut.draw ( p );
    }
}

bool GCtrlRadioButtonGroup::fwKeyPressEvent ( GKeyEvent *e )
{
    mpFocus = rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex );
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
    for ( GCtrlRadioButton* pItem = rbgHlo->m_itemList.first(); pItem; pItem=rbgHlo->m_itemList.next() )
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
    rbgHlo->m_itemList.append ( pItem );
}

void GCtrlRadioButtonGroup::slotLoseFocus()
{
}

void GCtrlRadioButtonGroup::slotGetFocus()
{
    rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->setFocus();
}

bool GCtrlRadioButtonGroup::moveFocusUp()
{
    int nIndex = rbgHlo->m_nCurItemIndex -1;
    if ( rbgHlo->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlRadioButtonGroup::moveFocusDown()
{
    int nIndex = rbgHlo->m_nCurItemIndex +1;
    if ( rbgHlo->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

void GCtrlRadioButtonGroup::moveFocus ( int toIndex )
{
    rbgHlo->m_itemList.at ( toIndex )->setFocus();

    rbgHlo->m_itemList.at ( rbgHlo->m_nCurItemIndex )->update();
    rbgHlo->m_itemList.at ( toIndex )->update();

    rbgHlo->m_nCurItemIndex = toIndex;
}

GMText* GCtrlRadioButtonGroup::getTitle()
{
    return &rbgHlo->m_txtTitle;
}

// have a nice day ^_^
// have a nice day ^_^
