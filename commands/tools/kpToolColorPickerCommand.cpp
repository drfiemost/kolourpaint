
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


#define DEBUG_KP_TOOL_COLOR_PICKER 0


#include <kpToolColorPickerCommand.h>

#include <KLocale>

#include <kpCommandEnvironment.h>
#include <kpDefs.h>


kpToolColorPickerCommand::kpToolColorPickerCommand (
        int mouseButton,
        const kpColor &newColor,
        const kpColor &oldColor,
        kpCommandEnvironment *environ)

    : kpCommand (environ),
      m_mouseButton (mouseButton),
      m_newColor (newColor),
      m_oldColor (oldColor)
{
}

kpToolColorPickerCommand::~kpToolColorPickerCommand ()
{
}


// public virtual [base kpCommand]
QString kpToolColorPickerCommand::name () const
{
    return i18n ("Color Picker");
}


// public virtual [base kpCommand]
kpCommandSize::SizeType kpToolColorPickerCommand::size () const
{
    return 0;
}


// public virtual [base kpCommand]
void kpToolColorPickerCommand::execute ()
{
    environ ()->setColor (m_mouseButton, m_newColor);
}

// public virtual [base kpCommand]
void kpToolColorPickerCommand::unexecute ()
{
    environ ()->setColor (m_mouseButton, m_oldColor);
}

