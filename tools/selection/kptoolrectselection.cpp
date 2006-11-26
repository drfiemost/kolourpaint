
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


#include <kptoolrectselection.h>

#include <klocale.h>

#include <kpdocument.h>
#include <kpmainwindow.h>
#include <kpselection.h>


kpToolRectSelection::kpToolRectSelection (kpMainWindow *mainWindow)
    : kpToolSelection (Rectangle,
                       i18n ("Selection (Rectangular)"),
                       i18n ("Makes a rectangular selection"),
                       Qt::Key_S,
                       mainWindow, "tool_rect_selection")			 
{
}

kpToolRectSelection::~kpToolRectSelection ()
{
}


// protected virtual [base kpToolSelection]
void kpToolRectSelection::createMoreSelectionAndUpdateStatusBar (
        const QPoint &/*accidentalDragAdjustedPoint*/,
        const QRect &normalizedRect)
{
    const QRect usefulRect = normalizedRect.intersect (document ()->rect ());
    document ()->setSelection (
        kpSelection (
            kpSelection::Rectangle, usefulRect,
            mainWindow ()->selectionTransparency ()));

    setUserShapePoints (startPoint (),
        QPoint (qMax (0, qMin (currentPoint ().x (), document ()->width () - 1)),
                qMax (0, qMin (currentPoint ().y (), document ()->height () - 1))));
}
