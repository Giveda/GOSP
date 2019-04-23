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

#include <config_giveda.h>

#ifdef CONFIG_gImage

#ifndef GIMAGEATTR_H
#define GIMAGEATTR_H

#include <gGlobal.h>

/*! @file  gImageAttr.h
 * @brief  GImageAttr 图片属性
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @struct GImageAttr
 * @brief 图片属性
 * 
 */
struct GImageAttr
{
    const char* szID;
    T_OFFSET w;
    T_OFFSET h;
};

#endif // GIMAGEATTR_H

#endif  //CONFIG_gImage
