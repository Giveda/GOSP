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

class GCtrlIconView;
class GCtrlButton;

class GMyForm : public GCtrlForm
{
public:
    GMyForm(GCtrlForm* parent=0, const char* name=0 );
    ~GMyForm();
private slots:
    void slotHighLighted(int);
    void slotBtn1BeenClicked();
    void slotSelected(int);
    void slotFocusChangedTo(int);
protected:
    virtual bool keyPressEvent(GKeyEvent* );
private:
    GCtrlIconView* m_pIconView;
    GCtrlButton* m_pBtn1;
};

#endif

// have a nice day ^_^
// have a nice day ^_^
