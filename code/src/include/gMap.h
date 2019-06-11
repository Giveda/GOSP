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

#ifndef GMAP_H
#define GMAP_H

#include <map>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

template<class key,  class value>
class GMap : public map<key, value>
{
public:
    typedef map<key, value> MAP_T;
    GMap()
    {

    }
    virtual ~GMap()
    {
    }
    bool contains(const key  &k) const
    {
        return map<key, value>::find(k) != map<key, value>::end();
    }

    bool insert( const key& k, const value v )
    {
        pair<typename map<key,value>::iterator,bool> ret;
        ret = map<key, value>::insert( pair<key, value>(k, const_cast<value>(v) ) );
        return ret.second;
    }

};





#endif 


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
