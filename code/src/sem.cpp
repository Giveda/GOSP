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

#include "sem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

int sem_creat( sem_t* sem, int pshared, int num )
{
    return sem_init( sem, pshared, num);
}

void del_sem( sem_t* sem )
{
    sem_destroy( sem );
}

int p( sem_t* sem )
{
    return sem_wait( sem );
}

int v( sem_t* sem )
{
    return sem_post(sem);
}

int getSemVal( sem_t* sem )
{
    int val=0;
    sem_getvalue(sem, &val );
    return val;
}

// have a nice day ^_^
