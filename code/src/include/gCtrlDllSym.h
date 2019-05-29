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

#ifndef CTRL_DLL_SYM_H
#define CTRL_DLL_SYM_H

#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#warning can not control which symbol is exported
#endif

#endif  //CTRL_DLL_SYM_H
