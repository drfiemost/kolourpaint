
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
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


#ifndef KP_EFFECTS_DIALOG_H
#define KP_EFFECTS_DIALOG_H


#include <kptoolpreviewdialog.h>


class QGroupBox;
class QStringList;
class QTimer;
class QVBoxLayout;

class KComboBox;

class kpColorEffectCommand;
class kpColorEffectWidget;
class kpMainWindow;


class kpEffectsDialog : public kpToolPreviewDialog
{
Q_OBJECT

public:
    kpEffectsDialog (bool actOnSelection,
                     kpMainWindow *parent,
                     const char *name = 0);
    virtual ~kpEffectsDialog ();

    virtual bool isNoOp () const;
    kpColorEffectCommand *createCommand () const;

protected:
    virtual QSize newDimensions () const;
    virtual QPixmap transformPixmap (const QPixmap &pixmap,
                                     int targetWidth, int targetHeight) const;

protected slots:
    void slotEffectSelected (int which);

    virtual void slotUpdate ();
    virtual void slotUpdateWithWaitCursor ();

    void slotDelayedUpdate ();

protected:
    static int s_lastEffectSelected;

    static int s_lastWidth, s_lastHeight;

    QTimer *m_delayedUpdateTimer;

    KComboBox *m_effectsComboBox;
    QGroupBox *m_settingsGroupBox;
    QVBoxLayout *m_settingsLayout;

    kpColorEffectWidget *m_colorEffectWidget;
};


#endif  // KP_EFFECTS_DIALOG_H
