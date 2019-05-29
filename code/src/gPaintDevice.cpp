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

#ifdef CONFIG_gPaintDevice

#ifndef GPAINTDEVICE_H
#define GPAINTDEVICE_H

#include <gSize.h>
#include <gCtrlDllSym.h>

/*! @file  gPaintDevice.h
 * @brief  GPaintDevice 可绘制对象的基类
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GPaintDevice
 * @brief 可绘制对象的基类
 * 
 */
class DLL_PUBLIC GPaintDevice : public GSize
{
public:
    /**
     * @brief 构造一个宽高均为0的空对象
     * 
     */
    GPaintDevice( );
    
    /**
     * @brief 根据指定的宽高去构造一个 paint device 对象
     * 
     * @param w ...
     * @param h ...
     */
    GPaintDevice(int w, int h );
    
    virtual ~GPaintDevice();
    
    /**
     * @brief 获取paint device的大小
     * 
     * @return const GSize&
     */
    const GSize&  size(); 
};

#endif // GPAINTDEVICE_H

#endif  //CONFIG_gPaintDevice

