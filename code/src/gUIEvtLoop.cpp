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

#include <gUIEvtLoop.h>
#include <gPaintEnginefactory.h>
#include <gPaintEngine.h>
#include <gLinuxFB.h>
#include <gCtrlView.h>
#include <gImage_p.h>
#include <gPainter_p.h>
#include <gInputHandler.h>
#include <eventlist_p.h>
#include <gEvent.h>
#include <gTimer.h>
#include <list>
#include <vector>
#include <pthread.h>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

class EvtLoop
{
public:
    EvtLoop(pthread_mutex_t m, pthread_t id)
        :mutex(m), id(id), bRun(true) {}
    virtual ~EvtLoop() {}

public:
    pthread_mutex_t   mutex;
    pthread_t id;
    bool bRun;
};

typedef vector<EvtLoop*>   LOOP_LST;
typedef list<long>  TIMER_LST;

void emitTimeout(GTimer* t)
{
    t->timeout.emit();
}

#ifdef CONFIG_QT_SIMULATOR
#undef signals
#undef slots
#include <gscreenonqt.h>
#include <sem.h>
#include <pthread.h>
sem_t g_sem_qt_obj_ready;
sem_t g_sem_qt_enterLoop;
#endif

#ifdef CONFIG_QT_SIMULATOR
#define EXIT_LOOP_EVENT ((GEvent::EVT_TYPE)(GEvent::USR_EVT_START+1))
void thread_entry(int (*start_routine) ( int argc, char** argv) )
{
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    
    start_routine(0, NULL );
    
    GEvent  *e = new GEvent(EXIT_LOOP_EVENT); 
    int pret = GUIEvtLoop::postEvent(e, uiEvtLp);
    if(0 != pret)
    {
        ERROR("post EXIT_LOOP_EVENT event to uiEvtLp failed[%d]\n", pret);
        exit(-1);
        return ;
    }
    
    return ;
}

int createThread(pthread_t *thread, const pthread_attr_t *attr,
                               int (*start_routine) ( int argc, char** argv) )
{
    int ret = pthread_create(thread, attr, (void *(*) (void *))thread_entry, (void*)start_routine);
    return ret;
}
#endif

LOOP_LST  loopLst;
EvtLoop *g_curLoop=NULL; 
TIMER_LST  timerLst;
GPaintEngine *g_engine=NULL;
GUIEvtLoop *uiEvtLp=NULL;
GCtrlView *g_ws=NULL;
GPaintDevice    *g_screen=NULL;

void dispatchEvent( GEvent* evt );

GUIEvtLoop::GUIEvtLoop( int , char**  )
    : GObject()
{
    char * pLibDir = getenv ( "GIVEDA_DIR" );
    if ( !pLibDir )
    {
        ERROR ( "please export GIVEDA_DIR=/path/to/GOSP_X.Y.Z/code\n" );
        exit(0);
    }
    m_strLibDir = pLibDir;
    INFO("GIVEDA_DIR=[%s]\n", m_strLibDir.c_str() );

    uiEvtLp = this;
    
#ifdef CONFIG_QT_SIMULATOR
    int ret = 0;
    ret = sem_creat(&g_sem_qt_obj_ready, 0,  0);
    if(-1 == ret) 
    {
        ERROR("create qt simulator obj ready semphore failed[%d]\n", ret);
        exit(-1);
        return ;
    }
    ret = sem_creat(&g_sem_qt_enterLoop, 0,  0);
    if(-1 == ret) 
    {
        ERROR("create qt simulator enter loop semphore failed[%d]\n", ret);
        exit(-1);
        return ;
    }
    int qt_thread_entry(int argc, char **argv);
    ret = createThread(&qtThreadID, NULL, qt_thread_entry);
    if(0 != ret)
    {
        ERROR("create qt simulator thread failed[%d]\n", ret);
        exit(-1);
        return ;
    }
#endif

#ifdef CONFIG_R_SCREEN_LINUX_FB
    char *dev = (char*)"/dev/fb0";
    g_screen = GLinuxFB::instance(dev);
    if(!g_screen)
    {
        ERROR("can not use [%s] as screen\n",  dev);
        exit(-1);
    }
    init_input_handlers();
#else
    g_screen = NULL;
#endif
    
    init_image_handlers();
    init_freetype();

    g_engine =  GPaintEngineFactory::createPaintEngine("");
    g_ws = new GCtrlView( "./pics");
    createSem();

    pthread_t id=pthread_self();
    pthread_mutex_t m;
    pthread_mutex_init(&m,NULL);

    g_curLoop = new EvtLoop(m, id);
    loopLst.push_back( g_curLoop );
    
#ifdef CONFIG_QT_SIMULATOR
    p(&g_sem_qt_obj_ready);
#endif
}

GUIEvtLoop::~GUIEvtLoop()
{
#ifdef CONFIG_QT_SIMULATOR
    del_sem(&g_sem_qt_enterLoop);
    del_sem(&g_sem_qt_obj_ready);
#endif
    
    pthread_mutex_destroy(&g_curLoop->mutex);

    delete g_curLoop;

    destroySem();
    
    delete g_ws;
    g_ws = NULL;
    
    delete g_engine;
    g_engine = NULL;
}

int GUIEvtLoop::exec()
{
    g_curLoop = loopLst.back();
    g_curLoop->id = pthread_self();
    GEvent *evt = NULL;
    while(g_curLoop->bRun)
    {
        evt = getEvt();
        dispatchEvent(evt);
    }

    return 0;
}

int GUIEvtLoop::enterLoop()
{
    pthread_mutex_t   mutex;
    pthread_mutex_init(&mutex,NULL);

    pthread_t id;
    EvtLoop *newLoop = new EvtLoop(mutex, id);
    loopLst.push_back( newLoop );
    if ( ( pthread_create ( &id, NULL, ( void * ( * ) ( void* ) ) exec, NULL ) ) != 0 )
    {
        ERROR ( "Create loop thread error!\n" );
        return -1;
    }

    pthread_mutex_lock ( &mutex );
    pthread_join(id, NULL);
    pthread_mutex_destroy(&mutex);

    loopLst.pop_back();
    g_curLoop =loopLst.back();

    return 0;
}

void GUIEvtLoop::exitLoop()
{
    EvtLoop *loop = loopLst.back();
    loop->bRun = false;
    pthread_t id = pthread_self();
    if(id != loop->id)
    {
        WARNING("should not call  enterLoop/exitLoop from different thread, may cause  dead lock\n");
    }
    pthread_mutex_unlock ( &loop->mutex );
}

void dispatchEvent ( GEvent* evt )
{
    switch( (int)evt->type() )
    {
    case GEvent::TIMER_EVT:
    {
        GTimerEvent *e = dynamic_cast<GTimerEvent*>(evt);
        GTimer *t = e->timer();
        TIMER_LST::iterator it = find(timerLst.begin(), timerLst.end(),  t->id() );
        if(it != timerLst.end() )
        {
            emitTimeout(t);
        }
    }
    break;
    case  GEvent::KEYPRESS_EVT:
    {
        GKeyEvent *e = dynamic_cast<GKeyEvent*>(evt);
        g_ws->fwKeyPress(e);
        break;
    }
    case  GEvent::KEYRELEASE_EVT:
    {
        GKeyEvent *e = dynamic_cast<GKeyEvent*>(evt);
        g_ws->keyReleaseEvent(e);
        break;
    }
    case GEvent::PAINT_EVT:
    {
        GPaintEvent *e = dynamic_cast<GPaintEvent*>(evt);
        g_ws->paintEvent(e);
        break;
    }
#ifdef CONFIG_QT_SIMULATOR
    case (GEvent::EVT_TYPE)EXIT_LOOP_EVENT:
    {
        uiEvtLp->quit();
        break;
    }
#endif
    default:
        break;
    }
}

void GUIEvtLoop::quit()
{
    exit(0);
}

void addTimer ( GTimer* t )
{
    timerLst.push_back(t->id() );
}

void deleteTimer ( GTimer* t )
{
    timerLst.remove(t->id() );
}

int GUIEvtLoop::postEvent ( GEvent* event, GObject* receiver, int priority )
{
    int ret = postEvent(event, priority, receiver);
    return ret;
}

int GUIEvtLoop::postEvent ( GEvent* event, int , GObject*  )
{
    putEvt(event);
    return 0;
}

#if defined(CONFIG_V_SCREEN_ENABLED) || !defined(CONFIG_R_SCREEN_DISABLED)
void GUIEvtLoop::setSize(int w, int h)
{
    g_ws->setSize(w, h);
    
#ifdef CONFIG_QT_SIMULATOR
    GScreenOnQt::instance()->doSetSize(w, h);
#endif
}
#endif
