
/*
   Copyright (c) 2007 Mike Gashler <gashlerm@yahoo.com>
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


#ifndef kpEffectHSVWidget_H
#define kpEffectHSVWidget_H


#include <kpEffectWidgetBase.h>


class KDoubleNumInput;


class kpEffectHSVWidget : public kpEffectWidgetBase
{
Q_OBJECT

public:
    kpEffectHSVWidget (bool actOnSelection, QWidget *parent);
    virtual ~kpEffectHSVWidget ();

    virtual QString caption () const;

    virtual bool isNoOp () const;
    virtual kpImage applyEffect (const kpImage &image);

    virtual kpEffectCommandBase *createCommand (
        kpCommandEnvironment *cmdEnviron) const;

protected:
    KDoubleNumInput *m_hueInput;
    KDoubleNumInput *m_saturationInput;
    KDoubleNumInput *m_valueInput;
};


#endif  // kpEffectHSVWidget_H
