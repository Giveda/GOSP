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

#ifndef GCTRLITEM_H
#define GCTRLITEM_H

#include "gMItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlForm;

class GCtrlImage : public GMImage
{
    G_DISABLE_ASSIGN ( GCtrlImage )
public:
    GCtrlImage ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlImage ( const GString & strPath, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlImage ( const GImage & img, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    bool isComplex();
};

class GCtrlPixmap : public GMPixmap
{
    G_DISABLE_ASSIGN ( GCtrlPixmap )
public:
    GCtrlPixmap ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    bool isComplex();
};

class GCtrlText : public GMText
{
    G_DISABLE_ASSIGN ( GCtrlText )
public:
    GCtrlText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    bool isComplex();
};

class GCtrlScrollText : public GMScrollText
{
    G_DISABLE_ASSIGN ( GCtrlScrollText )
public:
    GCtrlScrollText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    GCtrlScrollText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    bool isComplex();
private:
};

#endif


// nice day ^_^
// for fun ^_^
