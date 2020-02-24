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

#include <gPainter.h>
#include <gUIEvtLoop.h>
#include <gFontMetrics.h>
#include <gPainter_p.h>
#include <stdint.h>
#include <vector>
#include <gConstDefine.h>
#include <gGlobal.h>

extern GPaintEngine *g_engine;

void adjustSize ( GRect& dR, const GPaintDevice* d );

class GPainterState
{
public:
    GPoint     base;
    GBrush  brush;
    
    GFont   font;
    GColor  pen;
};

typedef vector<GPainterState>   STATE_STACK_T;

class GPainterMhL
{
public:
    GPainterMhL ( GPaintDevice* p );
    GPaintDevice *d;
    GPainterState  state;
    STATE_STACK_T  stack;
    GPixmap* pTxtBg;
    void drawBitmap ( FT_Bitmap* bitmap, int base_x, int base_y );
};

GPainterMhL::GPainterMhL ( GPaintDevice* p ) :d ( p )
{
    state.brush = GBrush ( GColor ( 250, 0, 0 ) );
    state.pen = GColor ( 0, 250, 0 );
    state.base = GPoint ( 0, 0 );
    state.font = GFont ( "Sans", 16 );
}

void bitBlt ( GPaintDevice* dst, GPoint dP, GPaintDevice* src, GRect sR, RasterOp rop, bool noBlend )
{
    if ( CopyROP != rop )
    {
        NOT_IMP();
        return ;
    }

    GPaintEngine * engine = g_engine;
    engine->setSource ( src );
    engine->setDestination ( dst );
    if(!noBlend && src->hasAlphaBuffer() )
    {
        engine->setAlphaType ( InlineAlpha );
    }
    else
    {
        engine->setAlphaType ( IgnoreAlpha );
    }
    engine->setRop ( rop );

    GRect dR ( dP, sR.size() );
    adjustSize ( dR, dst );
    sR.setSize ( dR.size() );

    engine->blt ( dP,  sR );
    return ;
}

inline void GPainterMhL::drawBitmap ( FT_Bitmap* bitmap, int base_x, int base_y )
{
    int x=0;
    int h, w;
    GRgb* row_addr=NULL;
    GRgb pen = state.pen.rgb();
    unsigned char r=pen & 0xff;
    unsigned char g= ( pen>>8 ) & 0xff;
    unsigned char b= ( pen>>16 ) & 0xff;
    unsigned char u8;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY:
        for ( h=0; h<bitmap->rows; h++ )
        {
            row_addr=pTxtBg->scanLine ( base_y+h );
            for ( w=0; w<bitmap->width; w++ )
            {
                x = base_x+w;
                u8 = bitmap->buffer[h*bitmap->pitch+w];
                if ( u8 )
                {
                    row_addr[x] = 0xff000000 | ( b*u8>>8 ) <<16 | ( g*u8>>8 ) <<8 | r*u8>>8;
                }
            }
        }
        break;
    case FT_PIXEL_MODE_MONO:
        for ( h=0; h<bitmap->rows; h++ )
        {
            row_addr=pTxtBg->scanLine ( base_y+h );
            for ( w=0; w<bitmap->width; w++ )
            {
                x = base_x+w;
                if ( bitmap->buffer[h*bitmap->pitch+w] )
                {
                    row_addr[x] = pen;
                }
            }
        }
        break;
    default:
        ERROR ( "pixel_mode [%d] not supported yet\n", bitmap->pixel_mode );
        break;
    }

}

GPainter::GPainter()
    :m_priv ( new GPainterMhL ( NULL ) )
{

}

GPainter::~GPainter()
{
    delete m_priv;
}

GPainter::GPainter ( GPaintDevice* p )
    :m_priv ( new GPainterMhL ( p ) )
{

}

GPaintDevice* GPainter::device() const
{
    return m_priv->d;
}

void GPainter::setDevice ( GPaintDevice* p )
{
    m_priv->d=p;
}

void GPainter::drawPixmap ( const GPixmap& pix, const GRect& dR, const GRect& sR )
{
    GRect dR1 ( dR );
    dR1.moveBy ( m_priv->state.base );

    bitBlt ( m_priv->d, dR1.topLeft(), const_cast<GPixmap*> ( &pix ), sR,  CopyROP );
}

void GPainter::drawPixmap ( int dx, int dy, const GPixmap& pix )
{
    drawPixmap ( pix, GRect ( dx, dy, pix.width(), pix.height() ),  GRect ( 0, 0, pix.width(), pix.height() ) );
}

void GPainter::drawPixmap ( int dx, int dy, const GPixmap& pix, int sx, int sy, int sw, int sh )
{
    drawPixmap ( pix, GRect ( dx, dy, sw, sh ),  GRect ( sx, sy,  sw, sh ) );
}

void GPainter::restore()
{
    m_priv->state = m_priv->stack.back();
    m_priv->stack.pop_back();
}

void GPainter::save()
{
    m_priv->stack.push_back ( m_priv->state );
}

void GPainter::translate ( int x, int y )
{
    GPoint *p = &m_priv->state.base;
    p->rx() += x;
    p->ry() += y;
}

void GPainter::fillRect ( const GRect& r, const GBrush& b )
{
    GRect dR ( r );
    dR.moveBy ( m_priv->state.base );

    adjustSize ( dR, m_priv->d );

    GPaintEngine * engine = g_engine;
    engine->setBrush ( b );
    engine->setDestination ( m_priv->d );
    engine->setAlphaType ( IgnoreAlpha );
    engine->setRop ( CopyROP );

    engine->fillRect ( dR );
}

void GPainter::fillRect ( const GRect& r )
{
    fillRect ( r, m_priv->state.brush );
}

void GPainter::drawImage ( int dx, int dy, const GImage& img, int sx, int sy, int sw, int sh )
{
    GRect dR ( dx, dy, sw, sh );
    dR.moveBy ( m_priv->state.base );
    bitBlt ( m_priv->d, dR.topLeft(), const_cast<GImage*> ( &img ), GRect ( sx,sy,sw,sh ),  CopyROP );
}

void GPainter::drawText ( int dX, int dY, int dW, int dH, int flags, const GString& str )
{
    GRect dR( dX, dY, dW, dH );
    dR.moveBy ( m_priv->state.base );
    adjustSize ( dR, m_priv->d );

    GPixmap  dev = grabWidget(m_priv->d, dR );
    m_priv->pTxtBg = &dev;
    GRect  bR(GPoint(0, 0), dR.size() );
    drawTextMhL ( bR, str, flags );

    bitBlt(m_priv->d, dR.topLeft(), m_priv->pTxtBg, bR, CopyROP, true );
}

void GPainter::drawTextMhL ( GRect dR, const GString& str, int flags )
{
    FT_GlyphSlot slot;
    uint16_t result=0, part1=0, part2=0, part3=0;
    char* pChar = str.utf8();
    int dX=dR.x(), dY=dR.y();
    FT_Face face = m_priv->state.font.face();
    FT_Error err;
    GFontMetrics  m( m_priv->state.font );
    int ascent = m.ascent(str);
    GSize s = m.size(flags, str );
    if( flags & Giveda::AlignHCenter)
    {
        int tmp = dR.width() - s.width();
        if( tmp >=2 )
        {
            dX += tmp/2;
        }
    }
    if( flags & Giveda::AlignVCenter)
    {
        int tmp = dR.height() - s.height();
        if( tmp >=2 )
        {
            dY += tmp/2;
        }
    }

    FT_Long  lH = FT_MulFix(face->height, face->size->metrics.y_scale);
    lH >>= 6;
    while ( *pChar != '\0' )
    {
        if ( ( *pChar&0xf0 ) == 0xe0 )
        {
            part1 = 0x0f & *pChar;
            pChar++;
            part2 = 0x3f & *pChar;
            pChar++;
            part3 = 0x3f & *pChar;
            result = part1<<12 | part2<<6 | part3;
            pChar++;
        }
        else
        {
            result = *pChar;
            pChar++;
        }

        err=FT_Load_Char ( face,  result, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT );
        if ( err )
        {
            ERROR ( "FT_Load_Char err[%d]\n", err );
            continue;
        }
        slot=face->glyph;
        FT_Pos  cW=(slot->metrics.horiBearingX+slot->metrics.width)>>6;
        if ( dR.right() < dX+cW-1 )
        {
            dX = dR.x();
            dY += lH;
        }
        if ( dR.bottom() < dY+lH-1 )
        {
            break;
        }
        int nY = dY + ascent;
        m_priv->drawBitmap ( &slot->bitmap,
                             dX + slot->bitmap_left,
                             nY - slot->bitmap_top <=0 ? 0: nY - slot->bitmap_top );
        dX += ( slot->advance.x>>6 );
    }

    return ;
}

void GPainter::fillRect ( int x, int y, int w, int h, const GBrush& b )
{
    fillRect ( GRect ( x,y,w,h ), b );
}

void GPainter::setFont ( const GFont & f )
{
    m_priv->state.font = f;
}

void GPainter::setPen ( const GColor & c )
{
    m_priv->state.pen = c;
}

void GPainter::setBrush ( const GBrush& b )
{
    m_priv->state.brush = b;
}

void adjustSize ( GRect& dR,  const GPaintDevice *d )
{
    int outW = dR.right() +1 - d->width();
    int outH = dR.bottom() +1 - d->height();
    int dW=dR.width(), dH=dR.height();
    if ( 0 < outW )
    {
        dW -= outW;
    }
    if ( 0 < outH )
    {
        dH -= outH;
    }
    dR.setSize ( GSize ( dW, dH ) );
}

GPixmap GPainter::grabWidget ( GPaintDevice* dev, const GRect & dR )
{
    GPixmap img(dR.width(), dR.height(), dev->depth() );
    ::bitBlt(&img, GPoint(0, 0), dev, dR, CopyROP, true );
    img.setAlphaBuffer( dev->hasAlphaBuffer() );

    return img;
}

void GPainter::bitBlt ( GPaintDevice* dst, GPoint dP, GPaintDevice* src, GRect sR, RasterOp rop, bool noBlend )
{
    if ( CopyROP != rop )
    {
        NOT_IMP();
        return ;
    }

    GPaintEngine * engine = g_engine;
    engine->setSource ( src );
    engine->setDestination ( dst );
    if(!noBlend && src->hasAlphaBuffer() )
    {
        engine->setAlphaType ( InlineAlpha );
    }
    else
    {
        engine->setAlphaType ( IgnoreAlpha );
    }
    engine->setRop ( rop );

    GRect dR ( dP, sR.size() );
    
    adjustSize ( dR, dst );
    sR.setSize ( dR.size() );

    engine->blt ( dP,  sR );
}

