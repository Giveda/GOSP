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

class GMProgressBarPrivate
{
public:
    GMProgressBarPrivate(GCtrlForm* form, GMItem* bar,  int totalSteps=100 )
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
    pbPriv = new GMProgressBarPrivate(form, this);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbPriv->m_imgProgressBg.setImage ( pStyle->pixmap ( pbPriv->m_imgProgressBg.name() ) );
    pbPriv->m_imgProgress.setImage ( pStyle->pixmap ( pbPriv->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbPriv->m_imgProgressCursor.name() ) ) );

    pbPriv->m_imgProgressBg.setWidth ( pbPriv->m_nTotalSteps*pbPriv->m_nStepLength +pbPriv->m_imgProgressCursor.width() );
    pbPriv->m_imgProgress.setWidth ( pbPriv->m_nProgress*pbPriv->m_nStepLength );
    pbPriv->m_imgProgressCursor.setX ( pbPriv->m_imgProgress.width() );
    setWidth ( pbPriv->m_imgProgressBg.width() );
}

GMProgressBar::GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent, const char* name )
    :GMItem ( form, parent, name )
{
    pbPriv = new GMProgressBarPrivate(form, this, totalSteps);
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( "GCtrlProgressBar" ) ) )
    {
        pAppStyle->appendProgressStyle();
    }

    pbPriv->m_imgProgressBg.setImage ( pStyle->pixmap ( pbPriv->m_imgProgressBg.name() ) );
    pbPriv->m_imgProgress.setImage ( pStyle->pixmap ( pbPriv->m_imgProgress.name() ) );
    setProgressCursorImage ( GImage ( pStyle->pixmap ( pbPriv->m_imgProgressCursor.name() ) ) );

    pbPriv->m_imgProgressBg.setWidth ( pbPriv->m_nTotalSteps*pbPriv->m_nStepLength +pbPriv->m_imgProgressCursor.width() );
    pbPriv->m_imgProgress.setWidth ( pbPriv->m_nProgress*pbPriv->m_nStepLength );
    pbPriv->m_imgProgressCursor.setX ( pbPriv->m_imgProgress.width() );
    setWidth ( pbPriv->m_imgProgressBg.width() );
}

GMProgressBar::~GMProgressBar()
{
    delete pbPriv;
}

int GMProgressBar::totalSteps() const
{
    return pbPriv->m_nTotalSteps;
}
int GMProgressBar::progress() const
{
    return pbPriv->m_nProgress;
}
int GMProgressBar::stepLength() const
{
    return pbPriv->m_nStepLength;
}
void GMProgressBar::setProgressColor ( GColor& c )
{
    pbPriv->m_progressColor=c;
}
void GMProgressBar::setProgressBgColor ( GColor& c )
{
    setPaletteBackgroundColor ( c );
}
void GMProgressBar::setProgressBgImage ( const GImage& p )
{
    pbPriv->m_imgProgressBg.setImage ( p );
}
void GMProgressBar::setProgressImage ( const GImage& p )
{
    pbPriv->m_imgProgress.setImage ( p );
}

void GMProgressBar::draw ( GPainter& p )
{
    pbPriv->m_imgProgressBg.draw ( p );
    pbPriv->m_imgProgress.draw ( p );
    pbPriv->m_imgProgressCursor.draw ( p );
}

void GMProgressBar::setProgressCursorImage ( const GImage& p )
{
    pbPriv->m_imgProgressCursor.setImage ( p );
    setHeight ( pbPriv->m_imgProgressCursor.height() );
    pbPriv->m_imgProgressBg.setY ( ( height()-pbPriv->m_imgProgressBg.height() ) /2 );
    pbPriv->m_imgProgress.setY ( ( height()-pbPriv->m_imgProgress.height() ) /2 );
}

void GMProgressBar::setProgressBgHeight ( const int h )
{
    pbPriv->m_imgProgressBg.setHeight ( h );
    pbPriv->m_imgProgress.setHeight ( h );
    pbPriv->m_imgProgressBg.setY ( ( height()-pbPriv->m_imgProgressBg.height() ) /2 );
    pbPriv->m_imgProgress.setY ( ( height()-pbPriv->m_imgProgress.height() ) /2 );
}

void GMProgressBar::slotSetProgress ( int progress )
{
    if ( pbPriv->m_nProgress == progress )
    {
        return ;
    }
    if ( 0>progress || progress > pbPriv->m_nTotalSteps )
    {
        return ;
    }
    pbPriv->m_nProgress = progress;
    pbPriv->m_imgProgress.setWidth ( pbPriv->m_nProgress*pbPriv->m_nStepLength );
    pbPriv->m_imgProgressCursor.setX ( pbPriv->m_imgProgress.width() );
    update();
    sigProgressChanged.emit ( pbPriv->m_nProgress );
}

void GMProgressBar::slotSetTotalSteps ( int totalSteps )
{
    pbPriv->m_nTotalSteps = totalSteps;
    pbPriv->m_imgProgressBg.setWidth ( pbPriv->m_nTotalSteps*pbPriv->m_nStepLength +pbPriv->m_imgProgressCursor.width() );
    setWidth ( pbPriv->m_imgProgressBg.width() );
}

void GMProgressBar::slotSetStepLength ( int nStepLen )
{
    pbPriv->m_nStepLength = nStepLen;
    pbPriv->m_imgProgressBg.setWidth ( pbPriv->m_nTotalSteps*pbPriv->m_nStepLength +pbPriv->m_imgProgressCursor.width() );
    pbPriv->m_imgProgress.setWidth ( pbPriv->m_nProgress*pbPriv->m_nStepLength );
    pbPriv->m_imgProgressCursor.setX ( pbPriv->m_imgProgress.width() );
    setWidth ( pbPriv->m_imgProgressBg.width() );
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
