
/*
   Copyright (c) 2003-2006 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#define DEBUG_KP_TOOL_RECTANGULAR_COMMAND 0


#include <kptoolrectangularcommand.h>

#include <qbitmap.h>
#include <qcursor.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpixmap.h>

#include <kdebug.h>
#include <klocale.h>

#include <kpbug.h>
#include <kpcolor.h>
#include <kpcommandhistory.h>
#include <kpdefs.h>
#include <kpdocument.h>
#include <kpmainwindow.h>
#include <kppainter.h>
#include <kppixmapfx.h>
#include <kptemppixmap.h>
#include <kptooltoolbar.h>
#include <kptoolwidgetfillstyle.h>
#include <kptoolwidgetlinewidth.h>
#include <kpview.h>
#include <kpviewmanager.h>


struct kpToolRectangularCommandPrivate
{
    kpToolRectangularBase::DrawShapeFunc drawShapeFunc;
    
    QRect rect;
    kpColor fcolor;
    int penWidth;
    kpColor bcolor;
    
    QPixmap *oldPixmapPtr;
};

kpToolRectangularCommand::kpToolRectangularCommand (const QString &name,
        kpToolRectangularBase::DrawShapeFunc drawShapeFunc,
        const QRect &rect,
        const kpColor &fcolor, int penWidth,
        const kpColor &bcolor,
        kpMainWindow *mainWindow)
    : kpNamedCommand (name, mainWindow),
      d (new kpToolRectangularCommandPrivate ())
{
    d->drawShapeFunc = drawShapeFunc;

    d->rect = rect;
    d->fcolor = fcolor;
    d->penWidth = penWidth;
    d->bcolor = bcolor;

    d->oldPixmapPtr = 0;
}

kpToolRectangularCommand::~kpToolRectangularCommand ()
{
    delete d->oldPixmapPtr;
    delete d;
}


// public virtual [base kpCommand]
int kpToolRectangularCommand::size () const
{
    return kpPixmapFX::pixmapSize (d->oldPixmapPtr);
}


// public virtual [base kpCommand]
void kpToolRectangularCommand::execute ()
{
    kpDocument *doc = document ();
    Q_ASSERT (doc);

    kpImage image = doc->getPixmapAt (d->rect);
    
    // Store Undo info.
    // OPT: I can do better with no brush
    Q_ASSERT (!d->oldPixmapPtr);
    d->oldPixmapPtr = new QPixmap ();
    *d->oldPixmapPtr = image;

    // Invoke shape drawing function passed in ctor.
    (*d->drawShapeFunc) (&image,
        0, 0, d->rect.width (), d->rect.height (),
        d->fcolor, d->penWidth, d->bcolor);

    doc->setPixmapAt (image, d->rect.topLeft ());
}

// public virtual [base kpCommand]
void kpToolRectangularCommand::unexecute ()
{
    kpDocument *doc = document ();
    Q_ASSERT (doc);

    Q_ASSERT (d->oldPixmapPtr);
    doc->setPixmapAt (*d->oldPixmapPtr, d->rect.topLeft ());

    delete d->oldPixmapPtr;
    d->oldPixmapPtr = 0;
}


#include <kptoolrectangularcommand.moc>
