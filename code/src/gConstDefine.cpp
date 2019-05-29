/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 * 
 * This program is an open-source software; and it is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. 
 * This program is not a free software; so you can not redistribute it and/or 
 * modify it without my authorization. If you only need use it for personal
 * study purpose(no redistribution, and without any  commercial behavior), 
 * you should accept and follow the GNU AGPL v3 license, otherwise there
 * will be your's credit and legal risks.  And if you need use it for any 
 * commercial purpose, you should first get commercial authorization from
 * me, otherwise there will be your's credit and legal risks. 
 *
 */

/*! @file  gConstDefine.h
 * @brief  debug/info/warning/error等 log 信息输出\n
 * ZL_DEBUG() debug信息输出\n
 * INFO() info信息输出\n
 * WARNING() warning信息输出\n
 * ERROR() error信息输出\n
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */
#ifndef GGCONSTDEFINE_H
#define GGCONSTDEFINE_H

#include <config_giveda.h>

#ifndef CONFIG_LINUX_SYSLOG

#include <stdio.h> 
#include <stdint.h>

#define MODULE_NAME "Giveda"
// #define FORMAT MODULE_NAME": ZL-DEBUG [%s, %s(), line %d] "
// #define ARGS __FILE__, __FUNCTION__, __LINE__
#define FORMAT MODULE_NAME": ZL-DEBUG [%s(), line %d] "
#define ARGS  __FUNCTION__, __LINE__

#if defined(CONFIG_DEBUG_ENABLED)
#define ZL_DEBUG(format, ...) fprintf(stdout, FORMAT format, ARGS, ##__VA_ARGS__)
#else
#define ZL_DEBUG(format, ...) 
#endif

// #define INFO(format, ...) fprintf(stdout, MODULE_NAME": [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
// #define ERROR(format, ...) fprintf(stderr, MODULE_NAME" error: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
// #define WARNING(format, ...) fprintf(stdout,  MODULE_NAME" warning: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)

#define INFO(format, ...) fprintf(stdout, MODULE_NAME": " format, ##__VA_ARGS__)
#define ERROR(format, ...) fprintf(stderr, MODULE_NAME" error: [%s(), line %d] " format, ARGS, ##__VA_ARGS__)
#define WARNING(format, ...) fprintf(stdout,  MODULE_NAME" warning: [%s(), line %d] " format, ARGS, ##__VA_ARGS__)

#endif  //ifndef CONFIG_LINUX_SYSLOG

#ifdef CONFIG_LINUX_SYSLOG

#include <syslog.h>
#include <stdint.h>

#define MODULE_NAME "Giveda"
// #define FORMAT MODULE_NAME": ZL-DEBUG [%s, %s(), line %d] "
// #define ARGS __FILE__, __FUNCTION__, __LINE__
#define FORMAT MODULE_NAME": ZL-DEBUG [%s(), line %d] "
#define ARGS  __FUNCTION__, __LINE__

#if defined(CONFIG_DEBUG_ENABLED)
#define ZL_DEBUG(format, ...) syslog(LOG_DEBUG, FORMAT format, ARGS, ##__VA_ARGS__)
#else
#define ZL_DEBUG(format, ...) 
#endif

// #define INFO(format, ...) fprintf(stdout, MODULE_NAME": [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
// #define ERROR(format, ...) fprintf(stderr, MODULE_NAME" error: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)
// #define WARNING(format, ...) fprintf(stdout,  MODULE_NAME" warning: [%s, %s(), line %d] " format, ARGS, ##__VA_ARGS__)

#define INFO(format, ...) syslog(LOG_INFO, MODULE_NAME": " format, ##__VA_ARGS__)
#define ERROR(format, ...) syslog(LOG_ERR, MODULE_NAME" error: [%s(), line %d] " format, ARGS, ##__VA_ARGS__)
#define WARNING(format, ...) syslog(LOG_WARNING,  MODULE_NAME" warning: [%s(), line %d] " format, ARGS, ##__VA_ARGS__)

#endif  //ifdef CONFIG_LINUX_SYSLOG

#define  NOT_IMP()  ERROR("not implemented yet\n")

#endif //GGCONSTDEFINE_H

