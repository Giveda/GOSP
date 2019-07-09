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

#ifndef GINPUTHANDLER_H
#define GINPUTHANDLER_H

#include <string>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

int init_input_handlers();
void deinit_input_handlers();

class  GInputHandlerHmq;

class GInputHandler
{
public:
    GInputHandler();
    GInputHandler ( const GInputHandler& other );
    ~GInputHandler();
    GInputHandler& operator= ( const GInputHandler& other );

    static int exec ( GInputHandlerHmq* data );
    int enterLoop();
    void exitLoop();

private:
    virtual string getEvtDevice();
    GInputHandlerHmq  *m_priv;
};

#endif 


// have a nice day ^_^
