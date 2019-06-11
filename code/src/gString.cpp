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

#include "gString.h"
#include <stdio.h>
#include <gConstDefine.h>
#include <gGlobal.h>

GString::GString():string()
{

}

GString::GString( const char* sz ): string(sz?sz:"")
{

}

GString::GString( const GString &s): string(s)
{

}

GString::GString ( const string& s ): string(s)
{

}

GString::~GString()
{

}

GString& GString::insert ( int idx, const char* arg )
{
    string::insert(idx, arg);
    return *this;
}

GString& GString::insert ( int idx, const GString& s )
{
    string::insert(idx, s);
    return *this;
}

GString& GString::remove ( int idx, int len )
{
    erase(idx, len);
    return *this;
}

GString GString::arg ( const GString& str )
{
    std::size_t pos=find_first_of('%');
    if(pos != string::npos)
    {
        replace(pos, 2, str);
    }

    return *this;
}

GString GString::arg ( const long int num )
{
    std::size_t pos=find_first_of('%');
    if(pos != string::npos)
    {
        char sz[100] = {0};
        sprintf(sz, "%ld",  num);
        replace(pos, 2, sz);
    }

    return *this;
}

void GString::fill ( const char c )
{
    replace(0, string::npos, length(), c);
}

bool GString::isEmpty() const
{
    return empty();
}

char* GString::utf8() const
{
    return const_cast<char*>(c_str() );
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
