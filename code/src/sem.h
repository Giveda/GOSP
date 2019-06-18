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

#ifndef SEM_H
#define SEM_H

#include <semaphore.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#ifdef __cplusplus
extern "C" {
#endif

int sem_creat( sem_t* sem, int pshared, int num );
void del_sem( sem_t* sem );
int p( sem_t* sem);
int v( sem_t* sem);
int getSemVal( sem_t* sem );

#ifdef __cplusplus
}
#endif

#endif

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
