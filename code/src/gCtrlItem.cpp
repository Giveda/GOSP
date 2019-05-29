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

#ifdef CONFIG_gCtrlItem

#ifndef GCTRLITEM_H
#define GCTRLITEM_H

#include <gMItem.h>

class GCtrlForm;

/*! @file  gCtrlItem.h
 * @brief  GCtrlImage 图片元素（能够自动缩放到用户所设置的尺寸大小） \n
 * GCtrlPixmap 图片元素（原样显示、不会自动缩放到用户所设置的尺寸大小）\n
 * GCtrlText 文字元素\n
 * GCtrlScrollText  滚动字幕
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlImage
 * @brief 图片元素（能够自动缩放到用户所设置的尺寸大小）；\n
 * GCtrlImage 与 GMImage 的区别在于： GCtrlImage 会主动将自己添加到 GCtrlForm 中，而 GMImage 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlImage ；而 GMImage 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GCtrlImage : public GMImage
{
    G_DISABLE_ASSIGN ( GCtrlImage )
public:
    /**
     * @brief 构造一个空的 GCtrlImage 图片元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlImage ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 构造一个 GCtrlImage 图片元素
     * 
     * @param imgID 图片ID
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlImage ( const uint16_t imgID, GCtrlForm* form, GMItem* parent = 0, const char* name = 0 );
    
    /**
     * @brief 构造一个 GCtrlImage 图片元素
     * 
     * @param img 图片
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlImage ( const GImage & img, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    DLL_LOCAL bool isComplex();
};

/*!
 * @class GCtrlPixmap
 * @brief 图片元素（原样显示，不会自动缩放到用户所设置的尺寸大小）；\n
 * GCtrlPixmap 与 GMPixmap 的区别在于： GCtrlPixmap 会主动将自己添加到 GCtrlForm 中，而 GMPixmap 则不会；因此开发者可以直接在 GCtrlForm 中使用 GCtrlPixmap ；而 GMPixmap 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GCtrlPixmap : public GMPixmap
{
    G_DISABLE_ASSIGN ( GCtrlPixmap )
public:
    /**
     * @brief 构造一个空的 GCtrlPixmap 图片元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlPixmap ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用指定的图片ID去构造一个 GCtrlPixmap 图片元素
     * 
     * @param imgID 图片ID
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlPixmap ( const uint16_t imgID, GCtrlForm* form, GMItem* parent = 0, const char* name = 0 );
    
    /**
     * @brief 使用指定的图片对象去构造一个 GCtrlPixmap 图片元素
     * 
     * @param img 图片
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlPixmap ( const GImage & img, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    DLL_LOCAL bool isComplex();
};

/*!
 * @class GCtrlText
 * @brief 文字元素；\n
 * GCtrlText 与 GMText 的区别在于： GCtrlText 会主动将自己添加到 GCtrlForm 中，而 GMText 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlText ；而 GMText 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GCtrlText : public GMText
{
    G_DISABLE_ASSIGN ( GCtrlText )
public:
    /**
     * @brief 构造一个空的 GCtrlText 文字元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的字符串去构造一个 GCtrlText 文字元素
     * 
     * @param str 字符串
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    DLL_LOCAL bool isComplex();
};

/*!
 * @class GCtrlScrollText
 * @brief 滚动字幕；\n
 * GCtrlScrollText 与 GMScrollText 的区别在于： GCtrlScrollText 会主动将自己添加到 GCtrlForm 中，而 GMScrollText 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlScrollText ；而 GMScrollText 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GCtrlScrollText : public GMScrollText
{
    G_DISABLE_ASSIGN ( GCtrlScrollText )
public:
    /**
     * @brief 构造一个空的 GCtrlScrollText 滚动字幕
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlScrollText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的字符串去构造一个 GCtrlScrollText 滚动字幕
     * 
     * @param str 字符串
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlScrollText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    DLL_LOCAL bool isComplex();
};

#endif

#endif  //CONFIG_gCtrlItem
