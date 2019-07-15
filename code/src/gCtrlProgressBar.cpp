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

class GMProgressBarLMQ
{
public:
    GMProgressBarLMQ(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
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
    pbHlo = new GMProgressBarLMQ(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbHlo->m_imgProgressBg.setImage ( pStyle->pixmap ( pbHlo->m_imgProgressBg.name() ) );
    pbHlo->m_imgProgress.setImage ( pStyle->pixmap ( pbHlo->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbHlo->m_imgProgressCursor.name() ) ) );

    pbHlo->m_imgProgressBg.setWidth ( pbHlo->m_nTotalSteps*pbHlo->m_nStepLength +pbHlo->m_imgProgressCursor.width() );
    pbHlo->m_imgProgress.setWidth ( pbHlo->m_nProgress*pbHlo->m_nStepLength );
    pbHlo->m_imgProgressCursor.setX ( pbHlo->m_imgProgress.width() );
    setWidth ( pbHlo->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbHlo = new GMProgressBarLMQ(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbHlo->m_imgProgressBg.setImage ( pStyle->pixmap ( pbHlo->m_imgProgressBg.name() ) );
    pbHlo->m_imgProgress.setImage ( pStyle->pixmap ( pbHlo->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbHlo->m_imgProgressCursor.name() ) ) );

    pbHlo->m_imgProgressBg.setWidth ( pbHlo->m_nTotalSteps*pbHlo->m_nStepLength +pbHlo->m_imgProgressCursor.width() );
    pbHlo->m_imgProgress.setWidth ( pbHlo->m_nProgress*pbHlo->m_nStepLength );
    pbHlo->m_imgProgressCursor.setX ( pbHlo->m_imgProgress.width() );
    setWidth ( pbHlo->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbHlo;
}

int GMProgressBar::totalSteps() const
{
    return pbHlo->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbHlo->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbHlo->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbHlo->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbHlo->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbHlo->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbHlo->m_imgProgressBg.draw ( p );
    pbHlo->m_imgProgress.draw ( p );
    pbHlo->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbHlo->m_imgProgressCursor.setImage ( p );
    setHeight ( pbHlo->m_imgProgressCursor.height() );
    pbHlo->m_imgProgressBg.setY ( ( height()-pbHlo->m_imgProgressBg.height() ) /2 );
    pbHlo->m_imgProgress.setY ( ( height()-pbHlo->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbHlo->m_imgProgressBg.setHeight ( h );
    pbHlo->m_imgProgress.setHeight ( h );
    pbHlo->m_imgProgressBg.setY ( ( height()-pbHlo->m_imgProgressBg.height() ) /2 );
    pbHlo->m_imgProgress.setY ( ( height()-pbHlo->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbHlo->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbHlo->m_nTotalSteps )
    {
        return ;
    }
    pbHlo->m_nProgress = progress;
    pbHlo->m_imgProgress.setWidth ( pbHlo->m_nProgress*pbHlo->m_nStepLength );
    pbHlo->m_imgProgressCursor.setX ( pbHlo->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbHlo->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbHlo->m_nTotalSteps = totalSteps;
    pbHlo->m_imgProgressBg.setWidth ( pbHlo->m_nTotalSteps*pbHlo->m_nStepLength +pbHlo->m_imgProgressCursor.width() );
    setWidth ( pbHlo->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbHlo->m_nStepLength = nStepLen;
    pbHlo->m_imgProgressBg.setWidth ( pbHlo->m_nTotalSteps*pbHlo->m_nStepLength +pbHlo->m_imgProgressCursor.width() );
    pbHlo->m_imgProgress.setWidth ( pbHlo->m_nProgress*pbHlo->m_nStepLength );
    pbHlo->m_imgProgressCursor.setX ( pbHlo->m_imgProgress.width() );
    
    setWidth ( pbHlo->m_imgProgressBg.width() );
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
// have a nice day ^_^
// have a nice day ^_^
