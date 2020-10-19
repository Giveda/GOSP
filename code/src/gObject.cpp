/*
 * Copyright (C) 2020  明心  <imleizhang@qq.com>
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

#include <gObject.h>
#include <gConstDefine.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <gGlobal.h>


class GObjectLMQ
{
public:
    GObjectLMQ ( GObject *p, const char* nm ) :m_parent ( p ), strName ( nm ) 
    {
        tid = pthread_self();
    }

    GObject *m_parent;
    string   strName;
    pthread_t  tid;
    list<SIGNAL_POINTER >  sLst;
};

GObject::GObject ( GObject *p,  const char *n )
    :m_priv ( new GObjectLMQ ( p, NULL==n?"":n ) )
{

}

GObject::~GObject()
{
    disconnectFromAllSignal();

    sigDestroyed.emit();

    delete m_priv;
}

class Slot_Is_CppSlot
{
private:
    GSlot vslot;
    
public:
    bool operator( ) ( GSlot* &obj1 )
    {
        if(obj1->type() != CPP_SLOT_TYPE)
        {
            return false;
        }
        
        return vslot == *obj1;
    }
    
    Slot_Is_CppSlot( const GSlot& s )
    :vslot(s)
    {
    }
};

static void saveSlot2Signal(SIGNAL_POINTER signal, GObject* , void* slot)
{
    GSlot *vslot = (GSlot*)slot;
    SIGNAL_TYPE_ITERATOR it = std::find_if ( signal->begin(), signal->end(),  Slot_Is_CppSlot( *vslot ) );
    if(it != signal->end() )
    {
        return ;
    }
    
    signal->push_back( vslot );
}

void GObject::saveSender(SIGNAL_POINTER signal)
{
    list<SIGNAL_POINTER >::iterator it = std::find(m_priv->sLst.begin(), m_priv->sLst.end(), signal);
    if(it != m_priv->sLst.end() )
    {
        return ;
    }

    m_priv->sLst.push_back(signal);
}

void GObject::deleteSender(SIGNAL_POINTER signal)
{
    m_priv->sLst.remove(signal);
}

static void deleteSlotFromSignal(SIGNAL_POINTER signal, GObject* receiver, void* slot)
{
    GSlot vslot(slot, receiver, CPP_SLOT_TYPE);
    signal->remove_if( Slot_Is_CppSlot( vslot ) ); 
}

class Receiver_Is
{
public:
    GObject* receiver;

    bool operator( ) ( GSlot* &obj1 )
    {
        if(obj1->type() != CPP_SLOT_TYPE)
        {
            return false;
        }
        
        return obj1->m_receiver == receiver;
    }

    Receiver_Is(GObject* r)
        :receiver(r)
    {

    }
};

static void deleteReceiverFromSignal(SIGNAL_POINTER signal, GObject* receiver)
{
    signal->remove_if( Receiver_Is(receiver) );
}

int GObject::privConnect(GObject* sender, SIGNAL_POINTER signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        printf ( "can not connect %s::%p to %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }

    saveSlot2Signal(signal, receiver, slot );
    
    receiver->saveSender(signal);
    
    return 0;
}

void GObject::disconnectFromAllSignal()
{
    list<SIGNAL_POINTER >::iterator it = m_priv->sLst.begin();
    while(it != m_priv->sLst.end() )
    {
        SIGNAL_POINTER signal=*it;
        deleteReceiverFromSignal(signal, this);
        ++it;
    }
}

int GObject::privDisconnect(GObject* sender, SIGNAL_POINTER signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        printf ( "can not disconnect sender %s::%p from receiver %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }
    
    deleteSlotFromSignal(signal, receiver, slot );
    
    receiver->deleteSender(signal);
    
    return 0;
}

const char* GObject::name() const
{
    return  m_priv->strName.c_str();
}

GObject* GObject::parent() const
{
    return m_priv->m_parent;
}

GObject& GObject::operator= ( const GObject&  )
{
    return *this;
}

GObject::GObject ( const GObject&  )
    :m_priv ( new GObjectLMQ ( NULL, "" ) )
{
}

bool GObject::event(GEvent* )
{
    return false;
}

pthread_t GObject::tid()
{
    return m_priv->tid;
}

void GSlot::operator() ( const GSlot& )
{
    
}

// nice day ^_^
