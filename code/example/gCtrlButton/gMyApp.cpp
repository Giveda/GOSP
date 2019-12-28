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

#include "gMyApp.h"

GMyApp::GMyApp ( int argc, char** argv )
    :GUIEvtLoop(argc, argv)
{
    // setViewSize
    setViewSize(900, 600);
    form2 = new GMyForm( NULL, "testForm2" );
    form2->setPosition(50, 50 );
    form2->loadBackgroundPixmap("./pics/frmBg.png");
    form2->setZ(3);
    form2->show();

    form  = new GMyForm( NULL, "testForm" );
    form->setPosition(200, 200);
    form->loadBackgroundPixmap("./pics/frmBg.png");
    form->setZ( 5 );
    form->show();
}

void GMyApp::keyPressEvent ( GKeyEvent* e )
{
    if( Giveda::Key_Space != e->key() )
    {
        return ;
    }
    static bool b=true;
    if(b)
    {
        form->setZ(3);
        form2->setZ(5);
        form2->setFocus();
    }
    else
    {
        form->setZ(5);
        form2->setZ(3);
        form->setFocus();
    }

    b = !b;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
