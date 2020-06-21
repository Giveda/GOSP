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

class GCtrlButton;
class GCtrlText;

class GMyForm : public GCtrlForm
{
public:
    GMyForm( GCtrlForm* parent=0, const char* name=0 );
    ~GMyForm();
private slots:
    void slotBtn1BeenClicked();
    void slotBtn2BeenClicked();
    void slotBtn3BeenClicked();
    void slotBtn4BeenClicked();
protected:
    virtual bool keyPressEvent(GKeyEvent* );
private:
    GCtrlButton* m_pBtn1;
    GCtrlButton* m_pBtn2;
    GCtrlButton* m_pBtn3;
    GCtrlButton* m_pBtn4;
    GCtrlText* m_txtInfo;
};

#endif

// nice day ^_^
// for fun ^_^
