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

#ifdef CONFIG_gString

#ifndef GSTRING_H
#define GSTRING_H

#include <string>
#include <gCtrlDllSym.h>
#include <string.h>

using namespace std;

/*! @file  gString.h
 * @brief  GString 字符串操作
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GString
 * @brief GString 字符串操作。对字符串进行各种运算处理。
 * 
 */
class DLL_PUBLIC GString: public string
{
public:
    GString();
    GString(const char *sz);
    GString( const GString & s); 
    GString( const string & s); 
    
    virtual ~GString();
    char* utf8 () const;
    
    GString arg ( const GString& str );
    GString arg ( const long int num );
    void fill ( const char c );
    GString& insert ( int idx, const char* arg );
    GString& insert ( int idx, const GString & s );
    GString& remove ( int idx, int len );
    bool isEmpty() const;
};

#endif // GSTRING_H

#endif  //CONFIG_gString
