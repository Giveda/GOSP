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

#include "gscreenonqt.h"
#include <QPainter>
#include <QKeyEvent>

#include <sem.h>
#include <gImage.h>
#include <gEvent.h>
#include <gConstDefine.h>
#include <gGlobal.h>

extern sem_t g_sem_qt_enterLoop;

GScreenOnQt *GScreenOnQt::instance()
{
    static GScreenOnQt *s_qtScreen = new GScreenOnQt;
    return s_qtScreen;
}

GScreenOnQt::GScreenOnQt(QWidget *parent)
    : QWidget(parent),
      m_img(NULL),
      m_needUpdate(false),
      m_img1(),
      m_strWinTittle(QString::fromUtf8("Giveda® 模拟器 (由giveda.com提供)") )
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setWindowTitle(m_strWinTittle);
    setFixedSize(1280, 720);

    connect(this, SIGNAL(sigRepaint(const QRegion &) ),
            this, SLOT(slotRepaint(const QRegion &) ), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(sigSetSize(int, int) ),
            this, SLOT(slotSetSize(int, int) ), Qt::BlockingQueuedConnection);
    
}

GScreenOnQt::~GScreenOnQt()
{

}

void GScreenOnQt::pEnterLoop()
{
}

void GScreenOnQt::vEnterLoopOnceWithImage(const GImage *gimg)
{
    static bool sb = true;
    if(false == sb)
    {
        return ;
    }

    sb = false;

    if(m_img)
    {
        delete m_img;
        m_img = NULL;
    }

    m_img = new QImage( (uchar *)gimg->scanLine(0), gimg->width(), gimg->height(), gimg->lineStep(), QImage::Format_ARGB32 );
    m_img1 = QImage( gimg->width(), gimg->height(), QImage::Format_ARGB32 );
    
}

void GScreenOnQt::doRepaint(const GRect &rect)
{
    if(m_img1 == *m_img)
    {
        return ;
    }
    
    QRect r(rect.x(), rect.y(), rect.width(), rect.height() );
    sigRepaint( QRegion(r) );
}

void GScreenOnQt::paintEvent(QPaintEvent *)
{
    if(false == m_needUpdate)
    {
        return ;
    }
    m_needUpdate = false;

    QPainter p(this);
    p.drawPixmap(0, 0,  QPixmap::fromImage(m_img1) );
}

void GScreenOnQt::slotRepaint(const QRegion &rgn)
{
    m_needUpdate = true;
    m_img1 = m_img->copy(0, 0, m_img->width(), m_img->height() );
    update(rgn);
}

void GScreenOnQt::doSetSize(int w, int h)
{
    emit sigSetSize(w, h);
}

void GScreenOnQt::slotSetSize(int w, int h)
{
    setFixedSize(w, h);
}

#ifdef CONFIG_QT_SIMULATOR
#undef signals
#undef slots
#undef emit
#include <gUIEvtLoop.h>
#endif

void GScreenOnQt::keyPressEvent ( QKeyEvent* qe )
{
    int32_t keyCode = qe->key();
    if (Qt::Key_0 <= keyCode && Qt::Key_9 >= keyCode) {
        keyCode = keyCode - Qt::Key_0 + Giveda::Key_0;
    } else if (Qt::Key_A <= keyCode && Qt::Key_Z >= keyCode) {
        keyCode = keyCode - Qt::Key_A + Giveda::Key_A;
    } else {
        switch(keyCode)
        {
        case Qt::Key_Left:
            keyCode = Giveda::Key_Left;
            break;
        case Qt::Key_Right:
            keyCode = Giveda::Key_Right;
            break;
        case Qt::Key_Up:
            keyCode = Giveda::Key_Up;
            break;
        case Qt::Key_Down:
            keyCode = Giveda::Key_Down;
            break;
        case Qt::Key_Escape:
            keyCode = Giveda::Key_Escape;
            break;
        case Qt::Key_Return:
            keyCode = Giveda::Key_Enter;
            break;
        case Qt::Key_Meta:
            keyCode = Giveda::Key_Meta;
            break;
        case Qt::Key_Home:
            keyCode = Giveda::Key_Home;
            break;
        case Qt::Key_Space:
            keyCode = Giveda::Key_Space;
            break;
        case Qt::Key_PageUp:
            keyCode = Giveda::Key_PageUp;
            break;
        case Qt::Key_PageDown:
            keyCode = Giveda::Key_PageDown;
            break;
        default:
            NOT_IMP();
            QWidget::keyPressEvent(qe);
            return ;
        }
    }
    GEvent  *ge = new GKeyEvent ( GEvent::KEYPRESS_EVT,
                                        keyCode,
                                        qe->isAutoRepeat() );
    int pret = GUIEvtLoop::postEvent(ge, uiEvtLp);
    if(0 != pret)
    {
        delete ge;
        ERROR("post keyPressEvent to uiEvtLp failed[%d]\n", pret);
        return ;
    }
    
    return ;
}

// have a nice day ^_^
// have a nice day ^_^
