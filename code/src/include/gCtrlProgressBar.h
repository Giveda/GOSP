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

#ifndef GCTRLPROGRESSBAR_H
#define GCTRLPROGRESSBAR_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GMProgressBarPrivate;


class GMProgressBar: public GMItem
{
    SET_CLASS_NAME(GMProgressBar)
    G_DISABLE_ASSIGN(GMProgressBar)

public:
    GMProgressBar ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    virtual ~GMProgressBar();

    int totalSteps () const;
    int progress () const;
    int stepLength () const;
    virtual void draw ( GPainter& );

    void setProgressColor(GColor &c);
    void setProgressBgColor(GColor &c);

    void setProgressBgImage(const GImage& p);
    void setProgressImage(const GImage& p);
    void setProgressCursorImage(const GImage& p);
    void setProgressBgHeight( const int h );

public slots:
    void slotSetProgress(int progress);
    void slotSetTotalSteps ( int totalSteps );
    void slotSetStepLength ( int nStepLen );
    
signals:
    GSignal<void(int)> sigProgressChanged;

private:
    GMProgressBarPrivate  *pbPriv;
};

class GCtrlProgressBar: public GMProgressBar
{
public:
    GCtrlProgressBar ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
};

#endif


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
