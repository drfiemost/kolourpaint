
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


#define DEBUG_KP_EFFECT_BLUR_SHARPEN 0


#include <kpEffectBlurSharpenCommand.h>

#include <kdebug.h>
#include <klocale.h>


kpEffectBlurSharpenCommand::kpEffectBlurSharpenCommand (kpEffectBlurSharpen::Type type,
        int strength,
        bool actOnSelection,
        kpCommandEnvironment *environ)
    : kpEffectCommandBase (kpEffectBlurSharpenCommand::nameForType (type),
                            actOnSelection, environ),
      m_type (type),
      m_strength (strength)
{
}

kpEffectBlurSharpenCommand::~kpEffectBlurSharpenCommand ()
{
}


// public static
QString kpEffectBlurSharpenCommand::nameForType (kpEffectBlurSharpen::Type type)
{
    if (type == kpEffectBlurSharpen::Blur)
        return i18n ("Soften");
    else if (type == kpEffectBlurSharpen::Sharpen)
        return i18n ("Sharpen");
    else
        return QString();
}


// protected virtual [base kpEffectCommandBase]
kpImage kpEffectBlurSharpenCommand::applyEffect (const kpImage &image)
{
    return kpEffectBlurSharpen::applyEffect (image, m_type, m_strength);
}

