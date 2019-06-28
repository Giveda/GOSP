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

#include <gObject.h>
#include <gConstDefine.h>
#include <string>
#include <algorithm>
#include <gConstDefine.h>
#include <gGlobal.h>


struct SenderPair
{
    SenderPair(GObject* _sender, SIGNAL_POINTER(void*) _signal )
        :sender(_sender), signal(_signal)
    { }

    bool operator==(const SenderPair &r ) const
    {
        return r.sender == sender && r.signal == signal;
    }

    GObject* sender;
    SIGNAL_POINTER(void*) signal;
};

class GObjectSelf
{
public:
    GObjectSelf ( GObject *p, const char* nm, bool r )
        :m_parent ( p ),
         strName ( nm ),
         reserved(r)
    {
        tid = pthread_self();

        sLst.clear();
        rLst.clear();
    }

    ~GObjectSelf()
    {
        sLst.clear();
        rLst.clear();
    }

    GObject *m_parent;
    string   strName;
    bool reserved;
    pthread_t  tid;

    list<SenderPair>  sLst;
    list<GObject*>  rLst;
};

GObject::GObject ( GObject *p,  const char *n, bool reserved )
    :m_priv ( new GObjectSelf ( p, NULL==n?"":n, reserved ) )
{
    if(false == reserved)
    {
        return ;
    }
}

GObject::~GObject()
{
    sigDestroyed.emit();

    destructAsSender();

    destructAsReceiver();

    delete m_priv;
}

void GObject::saveSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal)
{
    SenderPair sp(sender, signal);
    m_priv->sLst.push_back(sp);
}

void GObject::deleteSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal)
{
    m_priv->sLst.remove( SenderPair(sender, signal) );
}

class Receiver_Is
{
public:
    GObject* receiver;

    bool operator( ) ( GSlot* &obj1 )
    {
        return obj1->m_receiver == this->receiver;
    }

    bool operator( ) ( GObject* receiver )
    {
        return this->receiver == receiver;
    }

    Receiver_Is(GObject* r)
        :receiver(r)
    {

    }
};

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

int GObject::privConnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        WARNING ( "can not connect %s::%p to %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }

    GSlot *vslot = (GSlot*)slot;
    SIGNAL_TYPE_ITERATOR(void*) it = std::find_if ( signal->begin(), signal->end(),  Slot_Is_CppSlot( *vslot ) );
    if(it != signal->end() )
    {
        return -2;
    }

    signal->push_back( vslot );

    sender->saveReceiver(receiver);

    receiver->saveSenderPair( sender, signal );

    return 0;
}

void GObject::destructAsReceiver()
{
    list<SenderPair >::iterator it = m_priv->sLst.begin();
    while(it != m_priv->sLst.end() )
    {
        it->signal->remove_if( Receiver_Is(this) );
        it->sender->m_priv->rLst.remove_if( Receiver_Is(this) );
        ++it;
    }
}

class Sender_Is
{
public:
    GObject* sender;

    bool operator( ) ( SenderPair &obj1 )
    {
        return obj1.sender == sender;
    }

    Sender_Is(GObject* s)
        :sender(s)
    {

    }
};

void GObject::destructAsSender()
{
    list<GObject*>::iterator it = m_priv->rLst.begin();
    while(it != m_priv->rLst.end() )
    {
        GObject* receiver = *it;
        receiver->m_priv->sLst.remove_if( Sender_Is(this) );
        ++it;
    }
}

void GObject::saveReceiver(GObject *receiver)
{
    m_priv->rLst.push_front( receiver );
}

void GObject::deleteReceiver(GObject *receiver)
{
    list<GObject*>::iterator it = std::find(m_priv->rLst.begin(), m_priv->rLst.end(), receiver );
    if(it == m_priv->rLst.end() )
    {
        return ;
    }

    m_priv->rLst.erase(it);
}

int GObject::privDisconnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        WARNING ( "can not disconnect sender %s::%p from receiver %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }

    SIGNAL_TYPE_ITERATOR(void*) it = std::find_if ( signal->begin(), signal->end(),  Slot_Is_CppSlot( GSlot( slot, receiver, CPP_SLOT_TYPE) ) );
    if(it == signal->end() )
    {
        return -2;
    }

    signal->erase( it );

    sender->deleteReceiver(receiver);

    receiver->deleteSenderPair( sender, signal );

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

GObject::GObject ( const GObject& src )
    :m_priv ( new GObjectSelf ( NULL, "", src.m_priv->reserved ) )
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

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
