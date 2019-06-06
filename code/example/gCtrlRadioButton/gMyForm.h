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

#ifndef GMYFORM_H
#define GMYFORM_H

#include <gCtrlForm.h>
#include <gCtrlRadioButtonGroup.h>

class GCtrlButton;
class GCtrlText;

class GMyButtonGroup : public GCtrlRadioButtonGroup
{
public:
    friend class GMyForm;
    GMyButtonGroup( const GString& title, GCtrlForm*, GMItem* parent=0, const char* name=0 );
    ~GMyButtonGroup();
private slots:
    void slotRadioBtn1_checkedChanged();
    void slotRadioBtn2_checkedChanged();
    void slotRadioBtn3_checkedChanged();
    void slotRadioBtn4_checkedChanged();
    void slotRadioBtn5_checkedChanged();
private:
    GCtrlRadioButton* m_pRadioBtn1;
    GCtrlRadioButton* m_pRadioBtn2;
    GCtrlRadioButton* m_pRadioBtn3;
    GCtrlRadioButton* m_pRadioBtn4;
    GCtrlRadioButton* m_pRadioBtn5;
};

class GMyForm : public GCtrlForm
{
public:
    friend class GMyButtonGroup;
    GMyForm( GCtrlForm* parent = 0, const char* name = 0 );
    ~GMyForm();
private slots:
    void slotBtn1BeenClicked();
protected:
    virtual bool keyPressEvent(GKeyEvent* );
private:
    GCtrlButton* m_pBtn1;
    GMyButtonGroup* m_pRadioBtnGroup;
    GCtrlText* m_pTxt;
};

#endif
// have a nice day
// have a nice day
