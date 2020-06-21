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

class GMProgressBarHmq
{
public:
    GMProgressBarHmq(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
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
    pbMqs = new GMProgressBarHmq(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbMqs->m_imgProgressBg.setImage ( pStyle->pixmap ( pbMqs->m_imgProgressBg.name() ) );
    pbMqs->m_imgProgress.setImage ( pStyle->pixmap ( pbMqs->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbMqs->m_imgProgressCursor.name() ) ) );

    pbMqs->m_imgProgressBg.setWidth ( pbMqs->m_nTotalSteps*pbMqs->m_nStepLength +pbMqs->m_imgProgressCursor.width() );
    pbMqs->m_imgProgress.setWidth ( pbMqs->m_nProgress*pbMqs->m_nStepLength );
    pbMqs->m_imgProgressCursor.setX ( pbMqs->m_imgProgress.width() );
    setWidth ( pbMqs->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbMqs = new GMProgressBarHmq(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbMqs->m_imgProgressBg.setImage ( pStyle->pixmap ( pbMqs->m_imgProgressBg.name() ) );
    pbMqs->m_imgProgress.setImage ( pStyle->pixmap ( pbMqs->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbMqs->m_imgProgressCursor.name() ) ) );

    pbMqs->m_imgProgressBg.setWidth ( pbMqs->m_nTotalSteps*pbMqs->m_nStepLength +pbMqs->m_imgProgressCursor.width() );
    pbMqs->m_imgProgress.setWidth ( pbMqs->m_nProgress*pbMqs->m_nStepLength );
    pbMqs->m_imgProgressCursor.setX ( pbMqs->m_imgProgress.width() );
    setWidth ( pbMqs->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbMqs;
}

int GMProgressBar::totalSteps() const
{
    return pbMqs->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbMqs->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbMqs->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbMqs->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbMqs->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbMqs->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbMqs->m_imgProgressBg.draw ( p );
    pbMqs->m_imgProgress.draw ( p );
    pbMqs->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbMqs->m_imgProgressCursor.setImage ( p );
    setHeight ( pbMqs->m_imgProgressCursor.height() );
    pbMqs->m_imgProgressBg.setY ( ( height()-pbMqs->m_imgProgressBg.height() ) /2 );
    pbMqs->m_imgProgress.setY ( ( height()-pbMqs->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbMqs->m_imgProgressBg.setHeight ( h );
    pbMqs->m_imgProgress.setHeight ( h );
    pbMqs->m_imgProgressBg.setY ( ( height()-pbMqs->m_imgProgressBg.height() ) /2 );
    pbMqs->m_imgProgress.setY ( ( height()-pbMqs->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbMqs->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbMqs->m_nTotalSteps )
    {
        return ;
    }
    pbMqs->m_nProgress = progress;
    pbMqs->m_imgProgress.setWidth ( pbMqs->m_nProgress*pbMqs->m_nStepLength );
    pbMqs->m_imgProgressCursor.setX ( pbMqs->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbMqs->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbMqs->m_nTotalSteps = totalSteps;
    pbMqs->m_imgProgressBg.setWidth ( pbMqs->m_nTotalSteps*pbMqs->m_nStepLength +pbMqs->m_imgProgressCursor.width() );
    setWidth ( pbMqs->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbMqs->m_nStepLength = nStepLen;
    pbMqs->m_imgProgressBg.setWidth ( pbMqs->m_nTotalSteps*pbMqs->m_nStepLength +pbMqs->m_imgProgressCursor.width() );
    pbMqs->m_imgProgress.setWidth ( pbMqs->m_nProgress*pbMqs->m_nStepLength );
    pbMqs->m_imgProgressCursor.setX ( pbMqs->m_imgProgress.width() );
    
    setWidth ( pbMqs->m_imgProgressBg.width() );
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


// nice day ^_^
// for fun ^_^
