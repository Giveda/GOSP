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
#include <gTimer.h>
#include <algorithm>
#include <gConstDefine.h>
#include <gGlobal.h>

typedef struct
{
    int  key_code;
    uint16_t unicode;
    uint16_t shift_unicode;
    uint16_t ctrl_unicode;
}
KEYMAP_S;

KEYMAP_S  s_keyM[]=
{
    {   Giveda::Key_unknown,    0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Escape,     27      , 27      , 0xffff  },
    {   Giveda::Key_1,      '1'     , '!'     , 0xffff  },
    {   Giveda::Key_3,      '3'     , '#'     , 0xffff  },
    {   Giveda::Key_5,      '5'     , '%'     , 0xffff  },
    {   Giveda::Key_4,      '4'     , '$'     , 0xffff  },
    {   Giveda::Key_2,      '2'     , '@'     , 0xffff  },
    {   Giveda::Key_6,      '6'     , '^'     , 0xffff  },
    {   Giveda::Key_7,      '7'     , '&'     , 0xffff  },
    {   Giveda::Key_8,      '8'     , '*'     , 0xffff  },
    {   Giveda::Key_0,      '0'     , ')'     , 0xffff  },
    {   Giveda::Key_9,      '9'     , '('     , 0xffff  }, 
    {   Giveda::Key_Backspace,  8       , 8       , 0xffff  },
    {   Giveda::Key_W,      'w'     , 'W'     , 'W'-64  },
    {   Giveda::Key_Q,      'q'     , 'Q'     , 'Q'-64  },
    {   Giveda::Key_E,      'e'     , 'E'     , 'E'-64  },
    {   Giveda::Key_R,      'r'     , 'R'     , 'R'-64  },
    {   Giveda::Key_Y,      'y'     , 'Y'     , 'Y'-64  },
    {   Giveda::Key_T,      't'     , 'T'     , 'T'-64  },  
    {   Giveda::Key_U,      'u'     , 'U'     , 'U'-64  },
    {   Giveda::Key_I,      'i'     , 'I'     , 'I'-64  },
    {   Giveda::Key_O,      'o'     , 'O'     , 'O'-64  },
    {   Giveda::Key_P,      'p'     , 'P'     , 'P'-64  },
    {   Giveda::Key_Return,     13      , 13      , 0xffff  },
    {   Giveda::Key_S,      's'     , 'S'     , 'S'-64  },
    {   Giveda::Key_A,      'a'     , 'A'     , 'A'-64  },  
    {   Giveda::Key_D,      'd'     , 'D'     , 'D'-64  },
    {   Giveda::Key_F,      'f'     , 'F'     , 'F'-64  },
    {   Giveda::Key_H,      'h'     , 'H'     , 'H'-64  },
    {   Giveda::Key_G,      'g'     , 'G'     , 'G'-64  },
    {   Giveda::Key_K,      'k'     , 'K'     , 'K'-64  },
    {   Giveda::Key_L,      'l'     , 'L'     , 'L'-64  },
    {   Giveda::Key_J,      'j'     , 'J'     , 'J'-64  },
    {   Giveda::Key_X,      'x'     , 'X'     , 'X'-64  },
    {   Giveda::Key_Z,      'z'     , 'Z'     , 'Z'-64  },
    {   Giveda::Key_C,      'c'     , 'C'     , 'C'-64  },
    {   Giveda::Key_V,      'v'     , 'V'     , 'V'-64  },
    {   Giveda::Key_N,      'n'     , 'N'     , 'N'-64  },
    {   Giveda::Key_B,      'b'     , 'B'     , 'B'-64  },
    {   Giveda::Key_M,      'm'     , 'M'     , 'M'-64  },  
    {   Giveda::Key_Space,      ' '     , ' '     , 0xffff  },
    {   Giveda::Key_1,      '1'     , '1'     , 0xffff  },
    {   Giveda::Key_2,      '2'     , '2'     , 0xffff  },  
    {   Giveda::Key_3,      '3'     , '3'     , 0xffff  },
    {   Giveda::Key_4,      '4'     , '4'     , 0xffff  },
    {   Giveda::Key_5,      '5'     , '5'     , 0xffff  },
    {   Giveda::Key_6,      '6'     , '6'     , 0xffff  },
    {   Giveda::Key_7,      '7'     , '7'     , 0xffff  },
    {   Giveda::Key_8,      '8'     , '8'     , 0xffff  },
    {   Giveda::Key_0,      '0'     , '0'     , 0xffff  },
    {   Giveda::Key_9,      '9'     , '9'     , 0xffff  },
    {   Giveda::Key_Enter,      13      , 13      , 0xffff  },
    {   Giveda::Key_Up,    0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Prior,    0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Right,    0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Down,     0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Left,     0xffff  , 0xffff  , 0xffff  },
    {   Giveda::Key_Next,     0xffff  , 0xffff  , 0xffff  },
    {   0,          0xffff  , 0xffff  , 0xffff  }
};

#define  MIN_VALUE  32
#define  MAX_VALUE 126

GString GKeyEvent::text() const
{
    if ( MIN_VALUE > k || MAX_VALUE < k )
    {
        return GString();
    }

    return GString ( string ( 1, ( char ) k ) );
}

int GKeyEvent::ascii() const
{
    return unicode;
}

struct isKeyCode
{
    static uint16_t   keyCode;
    
    bool operator() ( const KEYMAP_S & item )
    {
        return item.key_code == keyCode;
    }
};

uint16_t isKeyCode::keyCode=0xFFFF;

GKeyEvent::GKeyEvent ( GEvent::EVT_TYPE type, int keyCode, bool autorep )
    : GEvent ( type ), k ( keyCode ), repeat ( autorep )
{
    static int size=sizeof ( s_keyM ) /sizeof ( KEYMAP_S );
    isKeyCode::keyCode = keyCode;
    KEYMAP_S *p = find_if ( s_keyM, s_keyM+size, isKeyCode() );
    if ( p<s_keyM+size )
    {
        unicode = p->unicode;
    }
    else
    {
        unicode = 0xFFFF;
    }
}

bool GKeyEvent::isEqual2 ( GEvent* e )
{
    if( e->type() !=  type() )
    {
        return false;
    }

    GKeyEvent *pe = dynamic_cast<GKeyEvent*>(e);

    return key() == pe->key();
}

bool GTimerEvent::isEqual2 ( GEvent* evt )
{
    if( !( evt->type() ==  type()) )
    {
        return false;
    }

    GTimerEvent *pe = dynamic_cast<GTimerEvent*>( evt );

    return  timer()->id() == pe->timer()->id();
}
bool GEvent::canComposite()
{
    return false;
}
void GEvent::composite ( GEvent* ) {  }
bool GEvent::isEqual2 ( GEvent* evt )
{
    return m_type == evt->type();
}

int GKeyEvent::key() const
{
    return k;
}

bool GKeyEvent::canComposite()
{
    return true;
}

bool GTimerEvent::canComposite()
{
    return true;
    
}

// a nice day ^_^
