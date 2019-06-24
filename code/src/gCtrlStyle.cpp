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

#include <gCtrlStyle.h>
#include <gConstDefine.h>
#include <gUIEvtLoop.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

GCtrlItemStyle::GCtrlItemStyle ( GCtrlAppStyle* pAppStyle )
    :m_pAppStyle ( pAppStyle )
{
}

GCtrlItemStyle::~GCtrlItemStyle()
{
    m_mapFont.clear();
    m_mapColor.clear();
}

void GCtrlItemStyle::setPicPath ( const GString& name, const GString& path )
{
    GString strPath = path;
    if ( isFileExist ( strPath ) )
    {
        m_mapPixmap[name] = GPixmap( strPath );
        return ;
    }
    strPath = m_pAppStyle->libPicPath() +"/"+name+".png";
    if ( isFileExist ( strPath ) )
    {
        m_mapPixmap[name] = GPixmap( strPath );
        return ;
    }

    strPath = uiEvtLp->libPath() +"/res/"+"default.bin";
    useResourceFile( strPath, name );
}

typedef struct {
    long long unsigned int num;
    char *buf;
}
PROTOCAL_S;

class ResItem
{
public:
    string  fileName;
    string  name;
    unsigned long long startPos;
    unsigned long long size;
};

string g_picPath;

string getString ( char* buf, int& index )
{
    string str = buf+index+sizeof(int);
    index += buf[index]+sizeof(int)+1;
    return str;
}

template<class NumType>
NumType getNum ( char* buf, int &index )
{
    NumType v = * ( ( NumType* ) ( buf+index ) );
    index += sizeof(v);
    return v;
}

int writePic( const ResItem & item,  FILE *fpIn )
{
    FILE *fp = fopen(item.fileName.c_str(), "wb");
    if(NULL == fp)
    {
        ERROR("fopen: [%s]\n", strerror(errno) );
        return -1;
    }
    char *buf = new char[item.size];
    unsigned long long off=0;
    fseek(fpIn, item.startPos, SEEK_SET);
    while(off < item.size)
    {
        off += fread(buf+off, 1, item.size-off, fpIn);
    }

    off=0;
    while(off < item.size)
    {
        off += fwrite(buf+off, 1, item.size-off, fp);
    }

    fclose(fp);
    return 0;
}

int GCtrlItemStyle::useResourceFile ( const GString& strPath,  const GString & name )
{
    g_picPath = "/tmp/tmp_";
    pid_t id = getpid();
    char szID[10]= {0};
    sprintf(szID, "%d", id);
    g_picPath += szID;
    string strCmd = "rm -rf ";
    strCmd += g_picPath + "; mkdir -p ";
    strCmd += g_picPath;
    system(strCmd.c_str() );
    FILE *fp = fopen(strPath.utf8(), "rb");
    if(NULL == fp)
    {
        ERROR("fopen [%s]: [%s]\n", strPath.utf8(), strerror(errno) );
        return -1;
    }
    PROTOCAL_S  b;
    int size=0;
    fread(&b.num, sizeof(b.num), 1, fp);
    fread(&size, sizeof(size), 1, fp);

    int len=b.num-sizeof(b.num)-sizeof(size);
    b.buf = new char[len];
    fread(b.buf, len, 1, fp);
    int index = 0;
    char *buf = b.buf;
    for(int i=0; i<size; i++)
    {
        ResItem item;
        item.fileName = getString(buf, index);
        item.name = getString(buf, index);
        item.startPos = getNum<unsigned long long>(buf, index);
        item.size = getNum<unsigned long long>(buf, index);
        if(name != item.name)
        {
            continue;
        }
        item.fileName = g_picPath +"/"+ item.fileName;
        writePic(item, fp);
        m_mapPixmap[name] = GPixmap( item.fileName );
        string strCmd = "rm -rf ";
        strCmd += g_picPath;
        system(strCmd.c_str() );
        break;
    }

    fclose(fp);

    return 0;
}

const GPixmap& GCtrlItemStyle::pixmap ( const GString& name )
{
    static GPixmap s_null;
    if ( m_mapPixmap.contains ( name ) )
    {
        return m_mapPixmap[name];
    }
    else
    {
        WARNING ( "item [%s] does not have a default pixmap\n", name.utf8() );
        return s_null;
    }
}

void GCtrlItemStyle::setFont ( const GString& name, const GFont& font )
{
    m_mapFont[name] = font;
}

const GFont GCtrlItemStyle::font ( const GString& name )
{
    if ( m_mapFont.contains ( name ) )
    {
        return m_mapFont[name];
    }
    else
    {
        WARNING ( "item [%s] does not have a default font\n", name.utf8() );
        return GFont();
    }
}

void GCtrlItemStyle::setColor ( const GString& name, const GColor& color )
{
    m_mapColor[name] = color;
}

const GColor GCtrlItemStyle::color ( const GString& name )
{
    if ( m_mapColor.contains ( name ) )
    {
        return m_mapColor[name];
    }
    else
    {
        WARNING ( "item [%s] does not have a default color\n", name.utf8() );
        return GColor ( 250, 250, 250 );
    }
}

bool GCtrlItemStyle::isFileExist ( const GString& strUtf8 )
{
    struct stat buf;
    if ( 0 != stat ( strUtf8.utf8(), &buf ) )
    {
        return false;
    }

    if ( S_ISREG ( buf.st_mode ) )
    {
        return true;
    }

    return false;
}

GCtrlAppStyle::GCtrlAppStyle()
{
    m_strPicDir = GString ( "%1%2" )
                  .arg ( uiEvtLp->libPath() )
                  .arg ( "/pics" );
}

GCtrlAppStyle::~GCtrlAppStyle()
{
    m_dictItemStyle.clear();
}

void GCtrlAppStyle::appendItemStyle ( const char* className, GCtrlItemStyle* pStyle )
{
    m_dictItemStyle.insert ( GString ( className ), pStyle );
}

GCtrlItemStyle* GCtrlAppStyle::itemStyle ( const char* className )
{
    if ( m_dictItemStyle.contains ( className ) )
    {
        return m_dictItemStyle[className];
    }

    return NULL;
}

void GCtrlDefaultAppStyle::appendButtonStyle()
{
    GCtrlItemStyle* btnStyle = new GCtrlItemStyle ( this );
    btnStyle->setPicPath ( "btnBg", appPicPath() +"/btnBg.png" );
    btnStyle->setPicPath ( "btnFocus", appPicPath() +"/btnFocus.png" );
    btnStyle->setFont ( "btnCaption", GFont ( "Sans", 20 ) );
    btnStyle->setColor ( "btnCaption", GColor ( 20, 20, 20 ) );
    appendItemStyle ( "GCtrlButton", btnStyle );
}

void GCtrlDefaultAppStyle::appendIconViewStyle()
{
    GCtrlItemStyle* iconViewStyle = new GCtrlItemStyle ( this );
    iconViewStyle->setPicPath ( "iconViewFocusIn", appPicPath() +"/iconViewFocusIn.png" );
    iconViewStyle->setPicPath ( "iconViewFocusOut", appPicPath() +"/iconViewFocusOut.png" );
    appendItemStyle ( "GCtrlIconView", iconViewStyle );
}

void GCtrlDefaultAppStyle::appendLineEditStyle()
{
    GCtrlItemStyle* pLineEditStyle = new GCtrlItemStyle ( this );
    pLineEditStyle->setPicPath ( "lineEditBg", appPicPath() +"/lineEditBg.png" );
    pLineEditStyle->setFont ( "lineEditText", GFont ( "Sans", 20 ) );
    pLineEditStyle->setColor ( "lineEditText", GColor ( 20, 20, 20 ) );
    appendItemStyle ( "GCtrlLineEdit", pLineEditStyle );
}

void GCtrlDefaultAppStyle::appendListBoxStyle()
{
    GCtrlItemStyle* pListBoxStyle = new GCtrlItemStyle ( this );
    pListBoxStyle->setPicPath ( "listBoxFocusIn", appPicPath() +"/listBoxFocusIn.png" );
    pListBoxStyle->setPicPath ( "listBoxFocusOut", appPicPath() +"/listBoxFocusOut.png" );
    appendItemStyle ( "GCtrlListBox", pListBoxStyle );
}

void GCtrlDefaultAppStyle::appendRadioButtonGroupStyle()
{
    GCtrlItemStyle* pRadioButtonGroupStyle = new GCtrlItemStyle ( this );
    pRadioButtonGroupStyle->setPicPath ( "radioButtonGroupFocusIn", appPicPath() +"/radioButtonGroupFocusIn.png" );
    pRadioButtonGroupStyle->setPicPath ( "radioButtonGroupFocusOut", appPicPath() +"/radioButtonGroupFocusOut.png" );
    appendItemStyle ( "GCtrlRadioButtonGroup", pRadioButtonGroupStyle );
}

void GCtrlDefaultAppStyle::appendRadioButtonStyle()
{
    GCtrlItemStyle* pRadioButtonStyle = new GCtrlItemStyle ( this );
    pRadioButtonStyle->setPicPath ( "radioButtonUnchecked", appPicPath() +"/radioButtonUnchecked.png" );
    pRadioButtonStyle->setPicPath ( "radioButtonChecked", appPicPath() +"/radioButtonChecked.png" );
    appendItemStyle ( "GCtrlRadioButton", pRadioButtonStyle );
}

void GCtrlDefaultAppStyle::appendMsgBoxStyle()
{
    GCtrlItemStyle* pMsgBoxStyle = new GCtrlItemStyle ( this );
    pMsgBoxStyle->setPicPath ( "msgBoxBg", appPicPath() +"/msgBoxBg.png" );
    pMsgBoxStyle->setFont ( "msgBoxTitle", GFont ( "Sans", 30 ) );
    pMsgBoxStyle->setColor ( "msgBoxTitle", GColor ( 250, 250, 20 ) );
    pMsgBoxStyle->setFont ( "msgBoxInfo", GFont ( "Sans", 26 ) );
    pMsgBoxStyle->setColor ( "msgBoxInfo", GColor ( 250, 250, 250 ) );
    appendItemStyle ( "GCtrlMsgBox", pMsgBoxStyle );
}

void GCtrlDefaultAppStyle::appendProgressStyle()
{
    GCtrlItemStyle* pItemStyle = new GCtrlItemStyle ( this );
    GString strName = "progressBg";
    pItemStyle->setPicPath ( strName, appPicPath() +"/"+strName+".png" );
    strName = "progress";
    pItemStyle->setPicPath ( strName, appPicPath() +"/"+strName+".png" );
    strName = "progressCursor";
    pItemStyle->setPicPath ( strName, appPicPath() +"/"+strName+".png" );
    appendItemStyle ( "GCtrlProgressBar", pItemStyle );
}


