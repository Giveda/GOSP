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

#ifndef GCTRLRADIOBUTTONGROUP_H
#define GCTRLRADIOBUTTONGROUP_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlRadioButtonGroup;
class GCtrlRadioButtonMhL;

class GCtrlRadioButton : public GMCtrlItem
{
    SET_CLASS_NAME(GCtrlRadioButton)
    G_DISABLE_ASSIGN(GCtrlRadioButton)

public:
    GCtrlRadioButton ( const GString& str, GCtrlForm* frm, GCtrlRadioButtonGroup* parent=0, const char* name=0, bool check=false );
    virtual ~GCtrlRadioButton();
    bool isChecked();
    void setChecked(bool b);
    void emitCheckedChanged();

protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );

signals:
    GSignal<void(void)> checkedChanged;

private:
    GCtrlRadioButtonMhL *rbLqH;
};

class GCtrlRadioButtonGroupMhL;

class GCtrlRadioButtonGroup : public GMContainerItem
{
    SET_CLASS_NAME(GCtrlRadioButtonGroup)
    G_DISABLE_ASSIGN(GCtrlRadioButtonGroup)

public:
    GCtrlRadioButtonGroup( const GString& title, GCtrlForm* frm, GMItem* parent=0, const char* name=0 );
    virtual ~GCtrlRadioButtonGroup();

    GMText* getTitle();

    void uncheckedOther(GCtrlRadioButton*);
    void appendItem(GCtrlRadioButton*);

protected:
    virtual bool fwKeyPressEvent(GKeyEvent*);
    virtual void paintEvent ( GPainter& );

private slots:
    void slotLoseFocus();
    void slotGetFocus();

private:
    bool moveFocusUp();
    bool moveFocusDown();
    void moveFocus( int toIndex );

private:
    GCtrlRadioButtonGroupMhL *rbgLqH;
};

#endif


