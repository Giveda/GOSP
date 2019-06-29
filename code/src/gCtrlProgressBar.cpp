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

#include "gCtrlProgressBar.h"
#include "gCtrlStyle.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GMProgressBarFles
{
public:
    GMProgressBarFles(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
        : m_progressColor ( 250, 0, 0 ), m_imgProgressBg ( form, bar, "progressBg" ), m_imgProgress ( form, bar, "progress" ), m_imgProgressCursor ( form, bar, "progressCursor" ), m_nProgress ( 0 ), m_nTotalSteps ( totalSteps ), m_nStepLength ( 1 )
    {}
    GColor m_progressColor;
    GMImage m_imgProgressBg;
    GMImage m_imgProgress;
    GMImage m_imgProgressCursor;
    int m_nProgress;
    int m_nTotalSteps;
    int m_nStepLength;
};

GMProgressBar::GMProgressBar ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbFhl = new GMProgressBarFles(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbFhl->m_imgProgressBg.setImage ( pStyle->pixmap ( pbFhl->m_imgProgressBg.name() ) );
    pbFhl->m_imgProgress.setImage ( pStyle->pixmap ( pbFhl->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbFhl->m_imgProgressCursor.name() ) ) );

    pbFhl->m_imgProgressBg.setWidth ( pbFhl->m_nTotalSteps*pbFhl->m_nStepLength +pbFhl->m_imgProgressCursor.width() );
    pbFhl->m_imgProgress.setWidth ( pbFhl->m_nProgress*pbFhl->m_nStepLength );
    pbFhl->m_imgProgressCursor.setX ( pbFhl->m_imgProgress.width() );
    setWidth ( pbFhl->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbFhl = new GMProgressBarFles(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbFhl->m_imgProgressBg.setImage ( pStyle->pixmap ( pbFhl->m_imgProgressBg.name() ) );
    pbFhl->m_imgProgress.setImage ( pStyle->pixmap ( pbFhl->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbFhl->m_imgProgressCursor.name() ) ) );

    pbFhl->m_imgProgressBg.setWidth ( pbFhl->m_nTotalSteps*pbFhl->m_nStepLength +pbFhl->m_imgProgressCursor.width() );
    pbFhl->m_imgProgress.setWidth ( pbFhl->m_nProgress*pbFhl->m_nStepLength );
    pbFhl->m_imgProgressCursor.setX ( pbFhl->m_imgProgress.width() );
    setWidth ( pbFhl->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbFhl;
}

int GMProgressBar::totalSteps() const
{
    return pbFhl->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbFhl->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbFhl->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbFhl->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbFhl->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbFhl->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbFhl->m_imgProgressBg.draw ( p );
    pbFhl->m_imgProgress.draw ( p );
    pbFhl->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbFhl->m_imgProgressCursor.setImage ( p );
    setHeight ( pbFhl->m_imgProgressCursor.height() );
    pbFhl->m_imgProgressBg.setY ( ( height()-pbFhl->m_imgProgressBg.height() ) /2 );
    pbFhl->m_imgProgress.setY ( ( height()-pbFhl->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbFhl->m_imgProgressBg.setHeight ( h );
    pbFhl->m_imgProgress.setHeight ( h );
    pbFhl->m_imgProgressBg.setY ( ( height()-pbFhl->m_imgProgressBg.height() ) /2 );
    pbFhl->m_imgProgress.setY ( ( height()-pbFhl->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbFhl->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbFhl->m_nTotalSteps )
    {
        return ;
    }
    pbFhl->m_nProgress = progress;
    pbFhl->m_imgProgress.setWidth ( pbFhl->m_nProgress*pbFhl->m_nStepLength );
    pbFhl->m_imgProgressCursor.setX ( pbFhl->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbFhl->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbFhl->m_nTotalSteps = totalSteps;
    pbFhl->m_imgProgressBg.setWidth ( pbFhl->m_nTotalSteps*pbFhl->m_nStepLength +pbFhl->m_imgProgressCursor.width() );
    setWidth ( pbFhl->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbFhl->m_nStepLength = nStepLen;
    pbFhl->m_imgProgressBg.setWidth ( pbFhl->m_nTotalSteps*pbFhl->m_nStepLength +pbFhl->m_imgProgressCursor.width() );
    pbFhl->m_imgProgress.setWidth ( pbFhl->m_nProgress*pbFhl->m_nStepLength );
    pbFhl->m_imgProgressCursor.setX ( pbFhl->m_imgProgress.width() );
    setWidth ( pbFhl->m_imgProgressBg.width() );
}
GCtrlProgressBar::GCtrlProgressBar ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMProgressBar ( form, parent, name )
{
    form->appendItem ( this );
}
GCtrlProgressBar::GCtrlProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMProgressBar ( totalSteps, form, parent, name )
{
    form->appendItem ( this );
}


// have a nice day ^_^
