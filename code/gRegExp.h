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

#ifdef CONFIG_gRegExp

#ifndef GREGEXP_H
#define GREGEXP_H

#include <string>
#include <gCtrlDllSym.h>

using namespace std;

class GRegExpPrivate;

/*! @file  gRegExp.h
 * @brief  GRegExp 正则表达式运算
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GRegExp
 * @brief GRegExp 正则表达式运算
 * 
 */
class DLL_PUBLIC GRegExp
{
public:
    /**
     * @brief 构建规则
     * 
     * @param s ...
     */
    GRegExp( const string& s );
    
    virtual ~GRegExp();
    
    /**
     * @brief 尝试在指定的字符串src中去匹配之前指定的规则；
     * 
     * @param src ...
     * @return int 匹配不到返回-1，否则返回子串的起始位置
     */
    int match(const string  &src );
    
private:
    GRegExpPrivate *m_priv;
};

#endif // GREGEXP_H

#endif  //CONFIG_gRegExp
