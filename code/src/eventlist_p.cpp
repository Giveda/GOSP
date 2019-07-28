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

#include <gEvent.h>
#include "eventlist_p.h"
#include "sem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

enum  SEM_IDX {
    EVT_PRODUCER=0,
    EVT_OPERATOR=1,
    SEM_MAX,
};

sem_t semID[SEM_MAX];
EVT_LST  evtLst;

void createSem()
{
    sem_creat(&semID[EVT_PRODUCER], 0,  0);

    sem_creat(&semID[EVT_OPERATOR], 0,  1);
}

void destroySem()
{
    for(int i=0; i<SEM_MAX; i++)
    {
        
        
        del_sem(&semID[i]);
    }
}


struct EqualEvent {
    static GEvent*  evt;
    
    bool operator() ( const GEvent * e)
    {
        return evt->isEqual2(const_cast<GEvent*>(e) );
    }
};

GEvent*  EqualEvent::evt=NULL;

void getBatch( GEvent *evt)
{
    if(false == evt->canComposite() )
    {
        return ;
        
    }

    EqualEvent::evt = evt;
    EVT_LST::iterator it=evtLst.begin();
    
    for(; it!=evtLst.end(); ++it)
    {
        if( false ==  evt->isEqual2( *it ) )
        {
            continue;
        }
        evt->composite(*it);
        p(semID+EVT_PRODUCER);
    }

    evtLst.remove_if( EqualEvent() );
}

GEvent* getEvt()
{
    p(semID+EVT_PRODUCER);

    p(semID+EVT_OPERATOR);
    GEvent *p= evtLst.front();
    evtLst.pop_front();
    getBatch(p);
    v(semID+EVT_OPERATOR);

    return p;
}

void putEvt( GEvent* e)
{
    p(semID+EVT_OPERATOR);
    evtLst.push_back(e);
    
    v(semID+EVT_OPERATOR);
    v(semID+EVT_PRODUCER);
}

