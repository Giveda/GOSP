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

#include "gMyForm.h"

#include <gConstDefine.h>
#include <gUIEvtLoop.h>
#include <gCtrlListBox.h>
#include <gCtrlButton.h>

GMyForm::GMyForm ( GCtrlForm* parent, const char* name )
    :GCtrlForm ( parent, name )
{
    m_pBtn1 = new GCtrlButton ( "Hide", this, NULL, "btn1" );
    m_pListBox = new GCtrlListBox ( this, NULL, "listBox1" );

    connect ( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked );
    connect ( m_pListBox, m_pListBox->highlighted_pi, this, &GMyForm::slotHighLighted );
    connect ( m_pListBox, m_pListBox->selected_pI, this, &GMyForm::slotSelected );
    connect ( m_pListBox, m_pListBox->focusChangedTo_pi, this, &GMyForm::slotFocusChangedTo );

    m_pListBox->setGeometry ( 0, m_pBtn1->height(), 100, 300 );
    m_pListBox->setRowNums ( 4 );
    m_pListBox->setColumnNums ( 1 );
    int i=0;
    GPixmap imgFile ( "./pics/file.png" );
    GPixmap imgFolder ( "./pics/folder.png" );
    while ( i<37 )
    {
        GString strTmp;
        GPixmap pix;
        if ( i%2 )
        {
            pix = imgFile;
            strTmp = GString ( "文件1文件2文件3" );
        }
        else
        {
            pix = imgFolder;
            strTmp = GString ( "目录" );
        }
        GString strTxt = GString ( "%1%2%3" )
                         .arg ( i )
                         .arg ( strTmp )
                         .arg ( i );
        m_pListBox->insertItem ( new GCtrlListBoxPixmap ( pix, strTxt, this, m_pListBox ) );
        i++;
    }
}

GMyForm::~GMyForm()
{
    if ( m_pListBox )
    {
        delete m_pListBox;
        m_pListBox = NULL;
    }
    if ( m_pBtn1 )
    {
        delete m_pBtn1;
        m_pBtn1 = NULL;
    }
}

void GMyForm::slotHighLighted ( int nIndex )
{
    GCtrlListBoxPixmap* pItem = ( GCtrlListBoxPixmap* ) m_pListBox->item ( nIndex );
    GString str = pItem->getTxt()->text().utf8();
    ZL_DEBUG ( "str is [%s]\n", str.utf8() );
}

void GMyForm::slotFocusChangedTo ( int nIndex )
{
    GCtrlListBoxPixmap* pItem = ( GCtrlListBoxPixmap* ) m_pListBox->item ( nIndex );
    GString str = pItem->getTxt()->text().utf8();
    ZL_DEBUG ( "str is [%s]\n", str.utf8() );
}

void GMyForm::slotSelected ( const GCtrlListBoxItem* p )
{
    GCtrlListBoxPixmap* pItem = ( GCtrlListBoxPixmap* ) p;
    GString str = pItem->getTxt()->text().utf8();
    ZL_DEBUG ( "str is [%s]\n", str.utf8() );
}

bool GMyForm::keyPressEvent ( GKeyEvent* e )
{
    ZL_DEBUG ( "start, className=[%s], name=[%s], key=[%d]\n", className(), name(), e->key() );
    bool bRet = true;
    switch ( e->key() )
    {
    case Giveda::Key_Escape:
        ZL_DEBUG ( "now app will quit\n" );
        uiEvtLp->quit();
        break;
    default:
        bRet = false;
    }

    return bRet;
}

void GMyForm::slotBtn1BeenClicked()
{
    ZL_DEBUG ( "start\n" );
    if ( m_pListBox->isVisible() )
    {
        m_pListBox->hide();
        m_pBtn1->setCaption ( "Show" );
    }
    else
    {
        m_pListBox->show();
        m_pBtn1->setCaption ( "Hide" );
    }
}

