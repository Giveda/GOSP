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

#ifndef CONSTDEFINE_H
#define CONSTDEFINE_H

#include <stdio.h>


#define MODULE_NAME "Giveda"
#define FORMAT MODULE_NAME": ZL-DEBUG [%s, %s(), line %d] "
#define ARGS __FILE__, __FUNCTION__, __LINE__

#ifdef CONFIG_DEBUG_ENABLED
#define ZL_DEBUG(format, ...) fprintf(stdout, FORMAT format, ARGS, ##__VA_ARGS__)
#else
#define ZL_DEBUG(format, ...)
#endif

#define INFO(format, ...) fprintf(stdout, MODULE_NAME": [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
#define ERROR(format, ...) fprintf(stdout, MODULE_NAME" error: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
#define WARNING(format, ...) fprintf(stdout,  MODULE_NAME" warning: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
#define  NOT_IMP()  ERROR("not implemented yet\n")

#endif


// reborn, a nice day ^_^
