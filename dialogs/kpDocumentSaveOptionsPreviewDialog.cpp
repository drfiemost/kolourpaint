
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

#define DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET 0


#include <kpDocumentSaveOptionsPreviewDialog.h>

#include <qevent.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kglobal.h>
#include <klocale.h>

#include <kpCommandSize.h>
#include <kpDefs.h>
#include <kpDocument.h>
#include <kpPixmapFX.h>
#include <kpResizeSignallingLabel.h>
#include <kpTransformPreviewDialog.h>


// protected static
const QSize kpDocumentSaveOptionsPreviewDialog::s_pixmapLabelMinimumSize (25, 25);


kpDocumentSaveOptionsPreviewDialog::kpDocumentSaveOptionsPreviewDialog (
    QWidget *parent )
    : kpSubWindow (parent),
#if 0
KDialogBase (parent, name, false/*non-modal*/,
                   i18n ("Save Preview"),
                   0/*no buttons*/),
#endif
      m_filePixmap (0),
      m_fileSize (0)
{
    setWindowTitle (i18nc ("@title:window", "Save Preview"));

    QWidget *baseWidget = this;//new QWidget (this);
    //setMainWidget (baseWidget);


    QGridLayout *lay = new QGridLayout ( baseWidget );
    lay->setMargin( KDialog::marginHint () );
    lay->setSpacing( KDialog::spacingHint ());

    m_filePixmapLabel = new kpResizeSignallingLabel (baseWidget);
    m_fileSizeLabel = new QLabel (baseWidget);


    m_filePixmapLabel->setMinimumSize (s_pixmapLabelMinimumSize);


    lay->addWidget (m_filePixmapLabel, 0, 0);
    lay->addWidget (m_fileSizeLabel, 1, 0, Qt::AlignHCenter);


    lay->setRowStretch (0, 1);


    connect (m_filePixmapLabel, SIGNAL (resized ()),
             this, SLOT (updatePixmapPreview ()));
}

kpDocumentSaveOptionsPreviewDialog::~kpDocumentSaveOptionsPreviewDialog ()
{
    delete m_filePixmap;
}


// public
QSize kpDocumentSaveOptionsPreviewDialog::preferredMinimumSize () const
{
    const int contentsWidth = 180;
    const int totalMarginsWidth = 2 * KDialog::marginHint ();

    return QSize (contentsWidth + totalMarginsWidth,
                  contentsWidth * 3 / 4 + totalMarginsWidth);
}


// public slot
void kpDocumentSaveOptionsPreviewDialog::setFilePixmapAndSize (const QImage &pixmap,
                                                               qint64 fileSize)
{
    delete m_filePixmap;
    m_filePixmap = new QImage (pixmap);

    updatePixmapPreview ();

    m_fileSize = fileSize;

    const kpCommandSize::SizeType pixmapSize = kpCommandSize::PixmapSize (pixmap);
    // (int cast is safe as long as the file size is not more than 20 million
    //  -- i.e. INT_MAX / 100 -- times the pixmap size)
    const int percent = pixmapSize ?
                            qMax (1,
                                  (int) ((kpCommandSize::SizeType) fileSize * 100 / pixmapSize)) :
                            0;
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kDebug () << "kpDocumentSaveOptionsPreviewDialog::setFilePixmapAndSize()"
               << " pixmapSize=" << pixmapSize
               << " fileSize=" << fileSize
               << " raw fileSize/pixmapSize%="
               << (pixmapSize ? (kpCommandSize::SizeType) fileSize * 100 / pixmapSize : 0)
               << endl;
#endif

    m_fileSizeLabel->setText (i18np ("1 byte (approx. %2%)", "%1 bytes (approx. %2%)",
                                     m_fileSize, percent));
}

// public slot
void kpDocumentSaveOptionsPreviewDialog::updatePixmapPreview ()
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kDebug () << "kpDocumentSaveOptionsPreviewDialog::updatePreviewPixmap()"
               << " filePixmapLabel.size=" << m_filePixmapLabel->size ()
               << " filePixmap.size=" << m_filePixmap->size ()
               << endl;
#endif

    if (m_filePixmap)
    {
        int maxNewWidth = qMin (m_filePixmap->width (),
                                m_filePixmapLabel->width ()),
            maxNewHeight = qMin (m_filePixmap->height (),
                                 m_filePixmapLabel->height ());

        double keepsAspect = kpTransformPreviewDialog::aspectScale (
            maxNewWidth, maxNewHeight,
            m_filePixmap->width (), m_filePixmap->height ());
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kDebug () << "\tmaxNewWidth=" << maxNewWidth
                   << " maxNewHeight=" << maxNewHeight
                   << " keepsAspect=" << keepsAspect
                   << endl;
    #endif


        const int newWidth = kpTransformPreviewDialog::scaleDimension (
            m_filePixmap->width (),
            keepsAspect,
            1,
            maxNewWidth);
        const int newHeight = kpTransformPreviewDialog::scaleDimension (
            m_filePixmap->height (),
            keepsAspect,
            1,
            maxNewHeight);
    #if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
        kDebug () << "\tnewWidth=" << newWidth
                   << " newHeight=" << newHeight
                   << endl;
    #endif


        QImage transformedPixmap =
            kpPixmapFX::scale (*m_filePixmap,
                               newWidth, newHeight);


        QImage labelPixmap (m_filePixmapLabel->width (),
                            m_filePixmapLabel->height (), QImage::Format_ARGB32_Premultiplied);
        labelPixmap.fill(QColor(Qt::transparent).rgba());
        kpPixmapFX::setPixmapAt (&labelPixmap,
            (labelPixmap.width () - transformedPixmap.width ()) / 2,
            (labelPixmap.height () - transformedPixmap.height ()) / 2,
            transformedPixmap);


        m_filePixmapLabel->setPixmap (QPixmap::fromImage(labelPixmap));
    }
    else
    {
        m_filePixmapLabel->setPixmap (QPixmap ());
    }
}


// protected virtual [base QWidget]
void kpDocumentSaveOptionsPreviewDialog::closeEvent (QCloseEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kDebug () << "kpDocumentSaveOptionsPreviewDialog::closeEvent()";
#endif

    QWidget::closeEvent (e);

    emit finished ();
}

// protected virtual [base QWidget]
void kpDocumentSaveOptionsPreviewDialog::moveEvent (QMoveEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kDebug () << "kpDocumentSaveOptionsPreviewDialog::moveEvent()";
#endif

    QWidget::moveEvent (e);

    emit moved ();
}

// protected virtual [base QWidget]
void kpDocumentSaveOptionsPreviewDialog::resizeEvent (QResizeEvent *e)
{
#if DEBUG_KP_DOCUMENT_SAVE_OPTIONS_WIDGET
    kDebug () << "kpDocumentSaveOptionsPreviewDialog::resizeEvent()";
#endif

    QWidget::resizeEvent (e);

    emit resized ();
}
