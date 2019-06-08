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

#include "gPaintEnginefactory.h"
#include <gConstDefine.h>
#include <gGlobal.h>

GPaintEngineFactory::GPaintEngineFactory()
{

}

GPaintEngineFactory::~GPaintEngineFactory()
{

}

GPaintEngine* GPaintEngineFactory::createPaintEngine ( const string &  )
{
    return new GSoftPaintEngine();
}


// have a nice day ^_^
// have a nice day ^_^
