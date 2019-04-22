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

#ifdef CONFIG_gPainter

#ifndef QPAINTER_H
#define QPAINTER_H

#include <gImage.h>
#include <gRect.h>
#include <gRegion.h>
#include <gColor.h>
#include <gFont.h>
#include <gBrush.h>

enum RasterOp   // raster op mode
{
    CopyROP,
    OrROP,
    XorROP,
    NotAndROP,
    EraseROP=NotAndROP,
    NotCopyROP,
    NotOrROP,
    NotXorROP,
    AndROP,  NotEraseROP=AndROP,
    NotROP,
    ClearROP,
    SetROP,
    NopROP,
    AndNotROP,
    OrNotROP,
    NandROP,
    NorROP,  LastROP=NorROP
};

/*! @file  gPainter.h
 * @brief  GPainter 提供GDI接口、较底层的绘制接口
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GPainter
 * @brief 提供GDI接口、较底层的绘制接口。开发者使用 GPainter::instance() 来获取 GPainter 对象。相关接口可以参考 Qt 文档
 * 
 */
class GPainter
{
public:
    enum TYPE {
        LOCAL=0,
        REMOTE,
        UNKNOWN,
    };
    
public:
    /**
     * @brief 获取 GPainter 对象
     * 
     * @return GPainter*
     */
    static GPainter* instance();
    
protected:
    GPainter();
    GPainter(const TYPE  t);
    
public:
    virtual ~GPainter();
    
    virtual void drawPixmap ( const GImage& pix, const GRect &dR, const GRect &sR )=0;
    virtual void drawPixmap ( T_OFFSET dx, T_OFFSET dy, const GImage &pix,  T_OFFSET sx=0, T_OFFSET sy=0, T_OFFSET sw=-1, T_OFFSET sh=-1 )=0;
    virtual void setClipRegion ( const GRegion & r)=0;
    virtual const GRegion&  clipRegion() const =0;
    virtual void save()=0;
    virtual void restore()=0;
    virtual void translate(T_OFFSET x, T_OFFSET y)=0;
    virtual void drawImage ( T_OFFSET dx, T_OFFSET dy, const GImage &img, T_OFFSET sx=0, T_OFFSET sy=0, T_OFFSET sw=-1, T_OFFSET sh=-1 )=0;
    virtual void setPen ( const GColor& c )=0;
    virtual void setFont ( const GFont& f )=0;
    virtual void setBrush ( const GBrush &b )=0;
    virtual void drawText ( T_OFFSET dX, T_OFFSET dY, T_OFFSET dW, T_OFFSET dH, int32_t flags, const GString& str )=0;
    virtual void fillRect(T_OFFSET x, T_OFFSET y, T_OFFSET w, T_OFFSET h, const GBrush &b)=0;
    virtual void fillRect( const GRect & r, const GBrush &b)=0;
    virtual void fillRect( const GRect & r)=0;

    TYPE type() const;
    
private:
    TYPE m_type;
};

#endif // QPAINTER_H

#endif  //CONFIG_gPainter
