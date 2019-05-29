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

#ifndef GIMAGE_H
#define GIMAGE_H

#ifdef CONFIG_V_SCREEN_ENABLED
#include <gString.h>
#endif
#include <gGlobal.h>
#include <gPaintDevice.h>
#include <gSharedData.h>

class GImageData;
class GImage;

#ifdef CONFIG_V_SCREEN_ENABLED
#pragma pack(1)
struct T_IMAGE
{
    GString imgID;
    T_SIZE size;
};
#pragma pack()
#endif

/*! @file  gImage.h
 * @brief  GImage 图片
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GImage
 * @brief 图片
 * 
 */
class DLL_PUBLIC GImage : public GPaintDevice
{
public:
    /**
     * @brief 构造一个空的图片对象，isNull() 将返回true
     * 
     */
    GImage();
    
    /**
     * @brief 使用指定的数字ID去构造图片对象
     * 
     * @param numID ...
     */
    GImage( const int16_t numID );
    
    GImage(const GImage & src);
    GImage & operator=(const GImage & r);
    virtual ~GImage();

    /**
     * @brief 使用指定的数字ID去加载图片
     * 
     * @param numID ...
     * @return int 0成功，非0失败
     */
    int load ( const int16_t numID );
    
    /**
     * @brief 返回本图片对象是否为空
     * 
     * @return bool
     */
    bool isNull() const;
    
    /**
     * @brief 缩放到指定的宽和高，返回缩放后的图片对象
     * 
     * @param width ...
     * @param height ...
     * @return GImage
     */
    GImage smoothScale ( int width, int height );
    
#ifdef CONFIG_V_SCREEN_ENABLED
    /**
     * @brief 返回字符串ID
     * 
     * @return const GString&
     */
    const GString& imageID() const;
#endif
    
    /**
     * @brief 返回数字ID
     * 
     * @return int16_t
     */
    int16_t numID() const;

private:
    DLL_LOCAL void initData();
    DLL_LOCAL void privDeref();
    DLL_LOCAL void privRef();
    
private:
    GImageData  *imgData;
};


#endif // GIMAGE_H

#endif  //CONFIG_gImage
