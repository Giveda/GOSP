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

#include <gConstDefine.h>
#include <gGlobal.h>

#ifndef CONFIG_GUI_MODE_NONE
#include <gUIEvtLoop.h>
#endif

#if defined(G_ASSERT)
#if defined(CONFIG_DEBUG_ENABLED)
void doQuit(const char *assertion, const char *file, int line)
{
    ERROR("ASSERT: [%s] at [%s, line %d]\n", assertion, file, line);
#ifndef CONFIG_GUI_MODE_NONE
    GUIEvtLoop::quit();
#endif
}
#endif   

void doNothing ( void ) {}
#endif   


bool mIsNull(float f)
{
    union U {
        float f;
        uint32_t u32;
    };
    U val;
    val.f = f;
    return val.u32 == 0;
}

int gRound(mreal d)
{
    return d >= mreal(0.0) ? int(d + mreal(0.5)) : int(d - int(d-1) + mreal(0.5)) + int(d-1);
}

bool gIsNear0(float f)
{
    return mAbs(f) <= 0.00001f;
}

bool mIsNull(double d)
{
    union U {
        double d;
        
        uint64_t u64;
    };
    U val;
    val.d = d;
    return val.u64 == uint64_t(0);
}

// nice day ^_^
// for fun ^_^
