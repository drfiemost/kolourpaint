
/*
   Copyright (c) 2003-2004 Clarence Dang <dang@kde.org>
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

#include <kiconloader.h>
#include <klocale.h>

#include <kptoolwidgetopaqueortransparent.h>


kpToolWidgetOpaqueOrTransparent::kpToolWidgetOpaqueOrTransparent (QWidget *parent)
    : kpToolWidgetBase (parent)
{
    setInvertSelectedPixmap (false);

    addOption (UserIcon ("option_opaque"), i18n ("Opaque")/*tooltip*/);
    startNewOptionRow ();
    addOption (UserIcon ("option_transparent"), i18n ("Transparent")/*tooltip*/);

    relayoutOptions ();
    setSelected (0, 0);
}

kpToolWidgetOpaqueOrTransparent::~kpToolWidgetOpaqueOrTransparent ()
{
}


// public
bool kpToolWidgetOpaqueOrTransparent::isOpaque () const
{
    return (selected () == 0);
}

// public
bool kpToolWidgetOpaqueOrTransparent::isTransparent () const
{
    return (!isOpaque ());
}


// protected slot virtual [base kpToolWidgetBase]
void kpToolWidgetOpaqueOrTransparent::setSelected (int row, int col)
{
    kpToolWidgetBase::setSelected (row, col);
    emit isOpaqueChanged (isOpaque ());
}


#include <kptoolwidgetopaqueortransparent.moc>
