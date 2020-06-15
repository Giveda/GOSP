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

#include "gCtrlItem.h"
#include "gCtrlForm.h"
#include <gConstDefine.h>
#include <gGlobal.h>

GCtrlImage::GCtrlImage ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMImage ( form, parent, name )
{
    form->appendItem ( this );
}

GCtrlImage::GCtrlImage ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMImage ( strPath, form, parent, name )
{
    form->appendItem ( this );
}

GCtrlImage::GCtrlImage ( const GImage & img, GCtrlForm* form, GMItem* parent, const char* name )
    : GMImage ( img, form, parent, name )
{
    form->appendItem ( this );
}

GCtrlPixmap::GCtrlPixmap ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMPixmap ( form, parent, name )
{
    form->appendItem ( this );
}

GCtrlPixmap::GCtrlPixmap ( const GString & strPath, GCtrlForm* form, GMItem* parent, const char* name )
    : GMPixmap ( strPath, form, parent, name )
{
    form->appendItem ( this );
}

GCtrlPixmap::GCtrlPixmap ( const GPixmap & pm, GCtrlForm* form, GMItem* parent, const char* name )
    : GMPixmap ( pm, form, parent, name )
{
    form->appendItem ( this );
}

GCtrlText::GCtrlText ( GCtrlForm* form, GMItem* parent, const char* name )
    : GMText ( form, parent, name )
{
    form->appendItem ( this );
}

GCtrlText::GCtrlText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    : GMText ( str, form, parent, name )
{
    form->appendItem ( this );
}

GCtrlScrollText::GCtrlScrollText ( GCtrlForm* form, GMItem* parent, const char* name )
    :GMScrollText ( form, parent, name )
{
    form->appendItem ( this );
}

GCtrlScrollText::GCtrlScrollText ( const GString & str, GCtrlForm* form, GMItem* parent, const char* name )
    :GMScrollText ( str, form, parent, name )
{
    form->appendItem ( this );
}

bool GCtrlImage::isComplex()
{
    return false;
}

bool GCtrlPixmap::isComplex()
{
    return false;
}

bool GCtrlText::isComplex()
{
    return false;
}

bool GCtrlScrollText::isComplex()
{
    return false;
}

// nice day ^_^
