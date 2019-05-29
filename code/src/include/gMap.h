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

#ifdef CONFIG_gMap

#ifndef QMAP_H
#define QMAP_H

#include <map>
#include <gCtrlDllSym.h>

using namespace std;

/*! @file  gMap.h
 * @brief  GMap Map数据结构
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GMap
 * @brief GMap Map数据结构
 * 
 */
template<class key,  class value> 
class DLL_PUBLIC GMap : public map<key, value>
{
public:
    typedef map<key, value> MAP_T;
    GMap()
    {
        
    }
    virtual ~GMap()
    {
        //     clear();
    }
    
    /**
     * @brief 是否包含指定的key
     * 
     * @param k  指定的key
     * @return bool
     */
    bool contains(const key  &k) const
    {
        return map<key, value>::count(k) == 1;
    }
    
    /**
     * @brief 插入key-value到map
     * 
     * @param k  指定的key
     * @param v  value
     * @return bool
     */
    bool insert( const key& k, const value v )
    {
        pair<typename map<key,value>::iterator,bool> ret;
        ret = map<key, value>::insert( pair<key, value>(k, const_cast<value>(v) ) );
        return ret.second;
    }
    
};

#endif // QMAP_H

#endif  //CONFIG_gMap
