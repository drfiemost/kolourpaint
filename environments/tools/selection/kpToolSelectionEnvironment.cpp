
/*
   Copyright (c) 2003-2007 Clarence Dang <dang@kde.org>
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


#include <kpToolSelectionEnvironment.h>

#include <kpMainWindow.h>
#include <kpImageSelectionTransparency.h>
#include <kpTextStyle.h>


struct kpToolSelectionEnvironmentPrivate
{
};

kpToolSelectionEnvironment::kpToolSelectionEnvironment (kpMainWindow *mainWindow)
    : kpToolEnvironment (mainWindow),
      d (new kpToolSelectionEnvironmentPrivate ())
{
}

kpToolSelectionEnvironment::~kpToolSelectionEnvironment ()
{
    delete d;
}


// public
kpImageSelectionTransparency kpToolSelectionEnvironment::imageSelectionTransparency () const
{
    return mainWindow ()->imageSelectionTransparency ();
}

// public
int kpToolSelectionEnvironment::settingImageSelectionTransparency () const
{
    return mainWindow ()->settingImageSelectionTransparency ();
}


// public
void kpToolSelectionEnvironment::deselectSelection () const
{
    mainWindow ()->slotDeselect ();
}

// public
QMenu *kpToolSelectionEnvironment::selectionToolRMBMenu () const
{
    return mainWindow ()->selectionToolRMBMenu ();
}


// public
void kpToolSelectionEnvironment::enableTextToolBarActions (bool enable) const
{
    mainWindow ()->enableTextToolBarActions (enable);
}

// public
kpTextStyle kpToolSelectionEnvironment::textStyle () const
{
    return mainWindow ()->textStyle ();
}

// public
int kpToolSelectionEnvironment::settingTextStyle () const
{
    return mainWindow ()->settingTextStyle ();
}
