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

#include "gInputHandler.h"
#include <gEvent.h>
#include <gConstDefine.h>
#include <gRegExp.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <eventlist_p.h>
#include "sem.h"
#include <fstream>
#include <gConstDefine.h>
#include <gGlobal.h>

Giveda::Key scanCode2KeyCode ( const int scanCode );

GInputHandler *s_kbd=NULL;

int init_input_handlers()
{
    s_kbd= new GInputHandler;
    int ret = s_kbd->enterLoop();
    return ret;
}

class  GInputHandlerPrivate
{
public:
    GInputHandlerPrivate ( const string & d=string() ) :run ( true ), dev ( d ) {}
    bool  run;
    string  dev;
    pthread_t id;
    int fd;
};

GInputHandler::GInputHandler()
    :m_priv ( new GInputHandlerPrivate() )
{

}

GInputHandler::GInputHandler ( const GInputHandler& )
{
    ERROR ( "you should not do so\n" );
}

GInputHandler::~GInputHandler()
{
    delete m_priv;
}

GInputHandler& GInputHandler::operator= ( const GInputHandler&  )
{
    ERROR ( "you should not do so\n" );
    return *this;
}

string GInputHandler::getEvtDevice()
{
    GRegExp  pattern("^N: Name=\".+keyboard");

    ifstream  fs;
    fs.open("/proc/bus/input/devices");

    bool b;
    string dev;
    string line;
    b=false;
    while( fs.good() )
    {
        getline(fs, line);
        dev = "key";
        if( -1 != pattern.match(line) )
        {
            b = true;
            break;
        }
    }
    if( !b)
    {
        fs.close();
        return dev;
    }

    GRegExp  patt("^H: Handlers=.*event");
    b=false;
    while( fs.good() )
    {
        getline(fs, line);
        if( -1 != patt.match(line) )
        {
            b = true;
            break;
        }
    }
    if( !b)
    {
        fs.close();
        return dev;
    }

    fs.close();
    string::size_type pos = line.find("event");
    if( string::npos == pos)
    {
        fs.close();
        return dev;
    }
    int d=0;
    sscanf(line.c_str()+pos, "event%d",  &d);
    char sz[100]= {0};
    sprintf(sz, "/dev/input/event%d", d);
    dev = sz;

    return dev;
}

int GInputHandler::exec ( GInputHandlerPrivate* data )
{
    pthread_setcancelstate ( PTHREAD_CANCEL_ENABLE, NULL );
    pthread_setcanceltype ( PTHREAD_CANCEL_DEFERRED, NULL );
    data->fd = open ( data->dev.c_str(), O_RDONLY );
    if ( -1 == data->fd )
    {
        ERROR ( "open [%s] failed: [%s]\n", data->dev.c_str(), strerror ( errno ) );
        return -1;
    }

    int ret;
    struct input_event  evt;
    memset(&evt, 0, sizeof(evt) );
    struct input_event  evtBak=evt;
    Giveda::Key   qtKey;
    ret = 0;
    while ( data->run )
    {
        ret = read ( data->fd, &evt, sizeof ( evt ) );
        if ( sizeof ( evt ) != ret ||  EV_KEY != evt.type )
        {
            continue;
        }
        qtKey = scanCode2KeyCode ( evt.code );
        GKeyEvent  *e = new GKeyEvent ( 1==evt.value?
                                        GEvent::KEYPRESS_EVT:GEvent::KEYRELEASE_EVT,
                                        qtKey,
                                        ( 1==evtBak.value && 1==evt.value ) );
        putEvt ( e );
        evtBak = evt;
    }

    close ( data->fd );
    return 0;
}

int GInputHandler::enterLoop()
{
    m_priv->run = true;
    m_priv->dev = getEvtDevice();
    if ( ( pthread_create ( &m_priv->id, NULL, ( void * ( * ) ( void* ) ) exec, ( void* ) m_priv ) ) != 0 )
    {
        ERROR ( "Create loop thread error!\n" );
        return -1;
    }

    return 0;
}

void GInputHandler::exitLoop()
{
    m_priv->run = false;
    close ( m_priv->fd );
    pthread_cancel ( m_priv->id );
    pthread_join ( m_priv->id, NULL );
}

Giveda::Key scanCode2KeyCode ( const int scanCode )
{
    Giveda::Key  key;
    switch ( scanCode )
    {
    case KEY_ESC:
        key=Giveda::Key_Esc;
        break;
    case KEY_BACKSPACE:
        key=Giveda::Key_Backspace;
        break;
    case KEY_ENTER:
        key=Giveda::Key_Enter;
        break;
    case KEY_LEFT:
        key=Giveda::Key_Left;
        break;
    case KEY_UP:
        key=Giveda::Key_Up;
        break;
    case KEY_RIGHT:
        key=Giveda::Key_Right;
        break;
    case KEY_DOWN:
        key=Giveda::Key_Down;
        break;
    case KEY_PAGEUP:
        key=Giveda::Key_PageUp;
        break;
    case KEY_PAGEDOWN:
        key=Giveda::Key_PageDown;
        break;
    case KEY_SPACE:
        key=Giveda::Key_Space;
        break;
    case KEY_1:
        key=Giveda::Key_1;
        break;
    case KEY_0:
        key=Giveda::Key_0;
        break;
    case KEY_3:
        key=Giveda::Key_3;
        break;
    case KEY_2:
        key=Giveda::Key_2;
        break;
    case KEY_4:
        key=Giveda::Key_4;
        break;
    case KEY_6:
        key=Giveda::Key_6;
        break;
    case KEY_5:
        key=Giveda::Key_5;
        break;
    case KEY_7:
        key=Giveda::Key_7;
        break;
    case KEY_8:
        key=Giveda::Key_8;
        break;
    case KEY_9:
        key=Giveda::Key_9;
        break;
    case KEY_A:
        key=Giveda::Key_A;
        break;
    case KEY_B:
        key=Giveda::Key_B;
        break;
    case KEY_C:
        key=Giveda::Key_C;
        break;
    case KEY_D:
        key=Giveda::Key_D;
        break;
    case KEY_E:
        key=Giveda::Key_E;
        break;
    case KEY_F:
        key=Giveda::Key_F;
        break;
    case KEY_G:
        key=Giveda::Key_G;
        break;
    case KEY_I:
        key=Giveda::Key_I;
        break;
    case KEY_H:
        key=Giveda::Key_H;
        break;
    case KEY_J:
        key=Giveda::Key_J;
        break;
    case KEY_K:
        key=Giveda::Key_K;
        break;
    case KEY_L:
        key=Giveda::Key_L;
        break;
    case KEY_M:
        key=Giveda::Key_M;
        break;
    case KEY_N:
        key=Giveda::Key_N;
        break;
    case KEY_O:
        key=Giveda::Key_O;
        break;
    case KEY_P:
        key=Giveda::Key_P;
        break;
    case KEY_Q:
        key=Giveda::Key_Q;
        break;
    case KEY_R:
        key=Giveda::Key_R;
        break;
    case KEY_T:
        key=Giveda::Key_T;
        break;
    case KEY_S:
        key=Giveda::Key_S;
        break;
    case KEY_V:
        key=Giveda::Key_V;
        break;
    case KEY_U:
        key=Giveda::Key_U;
        break;
    case KEY_W:
        key=Giveda::Key_W;
        break;
    case KEY_X:
        key=Giveda::Key_X;
        break;
    case KEY_Y:
        key=Giveda::Key_Y;
        break;
    case KEY_Z:
        key=Giveda::Key_Z;
        break;
    default:
        key = Giveda::Key_unknown;
        break;
    }

    return key;
}
// have a nice day
