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

#ifndef GSTRING_H
#define GSTRING_H

#include <string>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

class GString: public string
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

#endif 


// have a nice day ^_^
