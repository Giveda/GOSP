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

class GMProgressBarMhL
{
public:
    GMProgressBarMhL(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
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
    pbLqH = new GMProgressBarMhL(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbLqH->m_imgProgressBg.setImage ( pStyle->pixmap ( pbLqH->m_imgProgressBg.name() ) );
    pbLqH->m_imgProgress.setImage ( pStyle->pixmap ( pbLqH->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbLqH->m_imgProgressCursor.name() ) ) );

    pbLqH->m_imgProgressBg.setWidth ( pbLqH->m_nTotalSteps*pbLqH->m_nStepLength +pbLqH->m_imgProgressCursor.width() );
    pbLqH->m_imgProgress.setWidth ( pbLqH->m_nProgress*pbLqH->m_nStepLength );
    pbLqH->m_imgProgressCursor.setX ( pbLqH->m_imgProgress.width() );
    setWidth ( pbLqH->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbLqH = new GMProgressBarMhL(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbLqH->m_imgProgressBg.setImage ( pStyle->pixmap ( pbLqH->m_imgProgressBg.name() ) );
    pbLqH->m_imgProgress.setImage ( pStyle->pixmap ( pbLqH->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbLqH->m_imgProgressCursor.name() ) ) );

    pbLqH->m_imgProgressBg.setWidth ( pbLqH->m_nTotalSteps*pbLqH->m_nStepLength +pbLqH->m_imgProgressCursor.width() );
    pbLqH->m_imgProgress.setWidth ( pbLqH->m_nProgress*pbLqH->m_nStepLength );
    pbLqH->m_imgProgressCursor.setX ( pbLqH->m_imgProgress.width() );
    setWidth ( pbLqH->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbLqH;
}

int GMProgressBar::totalSteps() const
{
    return pbLqH->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbLqH->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbLqH->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbLqH->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbLqH->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbLqH->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbLqH->m_imgProgressBg.draw ( p );
    pbLqH->m_imgProgress.draw ( p );
    pbLqH->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbLqH->m_imgProgressCursor.setImage ( p );
    setHeight ( pbLqH->m_imgProgressCursor.height() );
    pbLqH->m_imgProgressBg.setY ( ( height()-pbLqH->m_imgProgressBg.height() ) /2 );
    pbLqH->m_imgProgress.setY ( ( height()-pbLqH->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbLqH->m_imgProgressBg.setHeight ( h );
    pbLqH->m_imgProgress.setHeight ( h );
    pbLqH->m_imgProgressBg.setY ( ( height()-pbLqH->m_imgProgressBg.height() ) /2 );
    pbLqH->m_imgProgress.setY ( ( height()-pbLqH->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbLqH->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbLqH->m_nTotalSteps )
    {
        return ;
    }
    pbLqH->m_nProgress = progress;
    pbLqH->m_imgProgress.setWidth ( pbLqH->m_nProgress*pbLqH->m_nStepLength );
    pbLqH->m_imgProgressCursor.setX ( pbLqH->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbLqH->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbLqH->m_nTotalSteps = totalSteps;
    pbLqH->m_imgProgressBg.setWidth ( pbLqH->m_nTotalSteps*pbLqH->m_nStepLength +pbLqH->m_imgProgressCursor.width() );
    setWidth ( pbLqH->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbLqH->m_nStepLength = nStepLen;
    pbLqH->m_imgProgressBg.setWidth ( pbLqH->m_nTotalSteps*pbLqH->m_nStepLength +pbLqH->m_imgProgressCursor.width() );
    pbLqH->m_imgProgress.setWidth ( pbLqH->m_nProgress*pbLqH->m_nStepLength );
    pbLqH->m_imgProgressCursor.setX ( pbLqH->m_imgProgress.width() );
    
    setWidth ( pbLqH->m_imgProgressBg.width() );
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
