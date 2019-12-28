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

#include "gRegExp.h"
#include <sys/types.h>
#include <regex.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GRegExpMhL
{
public:
    GRegExpMhL(const string  &s):pattern(s)
    {
        regex_t treg;
        int ret = regcomp(&treg, pattern.c_str(), REG_NEWLINE|REG_EXTENDED);
        if(0 != ret)
        {
            ERROR("regcomp [%s]\n", pattern.c_str() );
        }
        preg = treg;
    }
    string  pattern;
    regex_t preg;
};

GRegExp::GRegExp(const string  &s)
    :m_priv( new GRegExpMhL(s) )
{
}

GRegExp::~GRegExp()
{
    delete m_priv;
}

int GRegExp::match ( const string& src )
{
    regmatch_t  pmatch[1];
    int ret = regexec(&m_priv->preg, src.c_str(), 1, pmatch, 0 );

    if(REG_NOMATCH == ret)
    {
        return -1;
    }
    return pmatch[0].rm_so;
}

// have a nice day ^_^
// have a nice day ^_^
