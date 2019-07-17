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

class GMProgressBarSelf
{
public:
    GMProgressBarSelf(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
        : m_progressColor ( 250, 0, 0 ), 
        m_imgProgressBg ( form, bar, "progressBg" ), m_imgProgress ( form, bar, "progress" ), m_imgProgressCursor ( form, bar, "progressCursor" ), m_nProgress ( 0 ), m_nTotalSteps ( totalSteps ), m_nStepLength ( 1 )
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
    pbSpp = new GMProgressBarSelf(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbSpp->m_imgProgressBg.setImage ( pStyle->pixmap ( pbSpp->m_imgProgressBg.name() ) );
    pbSpp->m_imgProgress.setImage ( pStyle->pixmap ( pbSpp->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbSpp->m_imgProgressCursor.name() ) ) );

    pbSpp->m_imgProgressBg.setWidth ( pbSpp->m_nTotalSteps*pbSpp->m_nStepLength +pbSpp->m_imgProgressCursor.width() );
    pbSpp->m_imgProgress.setWidth ( pbSpp->m_nProgress*pbSpp->m_nStepLength );
    pbSpp->m_imgProgressCursor.setX ( pbSpp->m_imgProgress.width() );
    setWidth ( pbSpp->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbSpp = new GMProgressBarSelf(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbSpp->m_imgProgressBg.setImage ( pStyle->pixmap ( pbSpp->m_imgProgressBg.name() ) );
    pbSpp->m_imgProgress.setImage ( pStyle->pixmap ( pbSpp->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbSpp->m_imgProgressCursor.name() ) ) );

    pbSpp->m_imgProgressBg.setWidth ( pbSpp->m_nTotalSteps*pbSpp->m_nStepLength +pbSpp->m_imgProgressCursor.width() );
    pbSpp->m_imgProgress.setWidth ( pbSpp->m_nProgress*pbSpp->m_nStepLength );
    pbSpp->m_imgProgressCursor.setX ( pbSpp->m_imgProgress.width() );
    setWidth ( pbSpp->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbSpp;
}

int GMProgressBar::totalSteps() const
{
    return pbSpp->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbSpp->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbSpp->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbSpp->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbSpp->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbSpp->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbSpp->m_imgProgressBg.draw ( p );
    pbSpp->m_imgProgress.draw ( p );
    pbSpp->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbSpp->m_imgProgressCursor.setImage ( p );
    setHeight ( pbSpp->m_imgProgressCursor.height() );
    pbSpp->m_imgProgressBg.setY ( ( height()-pbSpp->m_imgProgressBg.height() ) /2 );
    pbSpp->m_imgProgress.setY ( ( height()-pbSpp->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbSpp->m_imgProgressBg.setHeight ( h );
    pbSpp->m_imgProgress.setHeight ( h );
    pbSpp->m_imgProgressBg.setY ( ( height()-pbSpp->m_imgProgressBg.height() ) /2 );
    pbSpp->m_imgProgress.setY ( ( height()-pbSpp->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbSpp->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbSpp->m_nTotalSteps )
    {
        return ;
    }
    pbSpp->m_nProgress = progress;
    pbSpp->m_imgProgress.setWidth ( pbSpp->m_nProgress*pbSpp->m_nStepLength );
    pbSpp->m_imgProgressCursor.setX ( pbSpp->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbSpp->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbSpp->m_nTotalSteps = totalSteps;
    pbSpp->m_imgProgressBg.setWidth ( pbSpp->m_nTotalSteps*pbSpp->m_nStepLength +pbSpp->m_imgProgressCursor.width() );
    setWidth ( pbSpp->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbSpp->m_nStepLength = nStepLen;
    pbSpp->m_imgProgressBg.setWidth ( pbSpp->m_nTotalSteps*pbSpp->m_nStepLength +pbSpp->m_imgProgressCursor.width() );
    pbSpp->m_imgProgress.setWidth ( pbSpp->m_nProgress*pbSpp->m_nStepLength );
    pbSpp->m_imgProgressCursor.setX ( pbSpp->m_imgProgress.width() );
    
    setWidth ( pbSpp->m_imgProgressBg.width() );
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


