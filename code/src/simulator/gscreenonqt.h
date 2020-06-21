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

#ifndef GSCREENONQT_H
#define GSCREENONQT_H

#include <QWidget>
#include <gRect.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GImage;

class GScreenOnQt : public QWidget
{
    Q_OBJECT
public:
    static GScreenOnQt* instance();
    virtual ~GScreenOnQt();

private:
    GScreenOnQt(QWidget *parent = 0);

public:
    void pEnterLoop();
    void vEnterLoopOnceWithImage( const GImage* gimg);
    void doRepaint(const GRect& rect);
    void doSetSize(int w, int h);

signals:
    void sigRepaint(const QRegion &rgn);
    void sigSetSize(int w, int h);
    
protected:
    virtual void paintEvent ( QPaintEvent * e );
    virtual void keyPressEvent( QKeyEvent * e );

private slots:
    void slotRepaint(const QRegion &rgn);
    void slotSetSize(int w, int h);
    
private:
    QImage *m_img;
    bool m_needUpdate;
    QImage m_img1;
    QString m_strWinTittle;
};

#endif 

// nice day ^_^
// for fun ^_^
