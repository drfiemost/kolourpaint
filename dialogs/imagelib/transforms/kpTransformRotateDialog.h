
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


#ifndef kpTransformRotateDialog_H
#define kpTransformRotateDialog_H


#include <QImage>
#include <qpoint.h>

#include <kpColor.h>
#include <kpTransformPreviewDialog.h>


class QButtonGroup;
class QRadioButton;

class KIntNumInput;


class kpTransformRotateDialog : public kpTransformPreviewDialog
{
Q_OBJECT

public:
    kpTransformRotateDialog (bool actOnSelection,
        kpTransformDialogEnvironment *_env,
        QWidget *parent);
    virtual ~kpTransformRotateDialog ();

private:
    static int s_lastWidth, s_lastHeight;
    static bool s_lastIsClockwise;
    static int s_lastAngleCustom;

    void createDirectionGroupBox ();
    void createAngleGroupBox ();

public:
    virtual bool isNoOp () const;
    int angle () const;  // 0 <= angle < 360 (clockwise);

private:
    virtual QSize newDimensions () const;
    virtual QImage transformPixmap (const QImage &pixmap,
                                    int targetWidth, int targetHeight) const;

private slots:
    void slotAngleCustomRadioButtonToggled (bool isChecked);
    virtual void slotUpdate ();

private slots:
    virtual void accept ();

private:
    QRadioButton *m_antiClockwiseRadioButton,
                 *m_clockwiseRadioButton;

    QButtonGroup *m_angleButtonGroup;
    QRadioButton *m_angle90RadioButton,
                 *m_angle180RadioButton,
                 *m_angle270RadioButton,
                 *m_angleCustomRadioButton;
    KIntNumInput *m_angleCustomInput;
};


#endif  // kpTransformRotateDialog_H
