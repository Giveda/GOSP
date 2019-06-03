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

#ifndef GGLOBAL_H
#define GGLOBAL_H

#include "gPortingQt.h"
#include "gConstDefine.h"

#define G_DISABLE_COPY_1(Class) \
private: \
    Class(const Class &) {} \
    Class &operator=(const Class &) { return *this; }

#define G_DISABLE_COPY(Class, BClass) \
private: \
    Class(const Class & r) : BClass(r) {} \
    Class &operator=(const Class &) { return *this; }

#define G_DISABLE_ASSIGN(Class) \
private: \
Class &operator=(const Class &) { return *this; }

typedef  int16_t T_OFFSET;

#if !defined(G_ASSERT)
void doNothing ( void );
#if defined(CONFIG_DEBUG_ENABLED)
void doQuit ( const char *assertion, const char *file, int line );
#    define G_ASSERT(cond) ((!(cond)) ? doQuit(#cond,__FILE__,__LINE__) : doNothing())
#  else
#    define G_ASSERT(cond) doNothing()
#  endif
#endif

template <typename T>
T mAbs(const T &t) {
    return t >= 0 ? t : -t;
}
template <typename T>
const T &gMin(const T &a, const T &b) {
    return (a < b) ? a : b;
}
template <typename T>
const T &gMax(const T &a, const T &b) {
    return (a < b) ? b : a;
}

int gRound(mreal d);

bool gIsNear0(float f);

bool mIsNull(double d);

bool mIsNull(float f);

class Giveda
{
public:
    enum Key
    {
        Key_Esc = 0x2000,   Key_Escape=Key_Esc,
        Key_Backspace, Key_BackSpace = Key_Backspace, Key_Back=Key_Backspace,
        Key_Enter, Key_Return = Key_Enter,
        Key_Left,
        Key_Up,
        Key_Right,
        Key_Down,
        Key_Home,
        Key_Meta, Key_Menu=Key_Meta,
        Key_PageUp, Key_Prior = Key_PageUp,
        Key_PageDown, Key_Next = Key_PageDown,
        Key_F1,
        Key_F2,
        Key_F3,
        Key_F4,
        Key_F5,
        Key_F6,
        Key_F7,
        Key_F8,
        Key_F9,
        Key_F10,
        Key_F11,
        Key_F12,
        Key_F13,
        Key_F14,
        Key_F15,
        Key_F16,
        Key_F17,
        Key_F18,
        Key_F19,
        Key_F20,
        Key_F21,
        Key_F22,
        Key_F23,
        Key_F24,
        Key_Space = 0x20, Key_Any = Key_Space,
        Key_0 = 0x30,
        Key_1 = 0x31,
        Key_2 = 0x32,
        Key_3 = 0x33,
        Key_4 = 0x34,
        Key_5 = 0x35,
        Key_6 = 0x36,
        Key_7 = 0x37,
        Key_8 = 0x38,
        Key_9 = 0x39,
        Key_A = 0x41,
        Key_B = 0x42,
        Key_C = 0x43,
        Key_D = 0x44,
        Key_E = 0x45,
        Key_F = 0x46,
        Key_G = 0x47,
        Key_H = 0x48,
        Key_I = 0x49,
        Key_J = 0x4a,
        Key_K = 0x4b,
        Key_L = 0x4c,
        Key_M = 0x4d,
        Key_N = 0x4e,
        Key_O = 0x4f,
        Key_P = 0x50,
        Key_Q = 0x51,
        Key_R = 0x52,
        Key_S = 0x53,
        Key_T = 0x54,
        Key_U = 0x55,
        Key_V = 0x56,
        Key_W = 0x57,
        Key_X = 0x58,
        Key_Y = 0x59,
        Key_Z = 0x5a,
        Key_unknown = 0xffff
    };

    enum AlignmentFlags {
        AlignLeft   = 0x0001,
        AlignRight  = 0x0002,
        AlignHCenter   = 0x0004,
        AlignTop = 0x0008,
        AlignBottom = 0x0010,
        AlignVCenter   = 0x0020,
        AlignCenter = AlignVCenter | AlignHCenter,

        SingleLine  = 0x0040,
        DontClip = 0x0080,
        ExpandTabs  = 0x0100,
        ShowPrefix  = 0x0200,
        WordBreak   = 0x0400,
        DontPrint   = 0x1000
    };

};

class  GPlatformInfo {
public:
    enum Sizes {
        WordSize = (sizeof(char *)<<3)
    };

    enum Endian {
        BigEndian,
        LittleEndian
    };
};

uint8_t  gRound();

#endif
