
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


#define DEBUG_KP_THUMBNAIL 0


#include <kpThumbnail.h>

#include <QAction>
#include <QLayout>

#include <kdebug.h>
#include <klocale.h>

#include <kpDefs.h>
#include <kpDocument.h>
#include <kpMainWindow.h>
#include <kpThumbnailView.h>
#include <kpTool.h>


struct kpThumbnailPrivate
{
    kpMainWindow *mainWindow;
    kpThumbnailView *view;
    QHBoxLayout *lay;
};

kpThumbnail::kpThumbnail (kpMainWindow *parent)
    : kpSubWindow (parent),
      d (new kpThumbnailPrivate ())
{
    Q_ASSERT (parent);

    d->mainWindow = parent;
    d->view = 0;
    d->lay = new QHBoxLayout (this);


    setMinimumSize (64, 64);


    updateCaption ();
}

kpThumbnail::~kpThumbnail ()
{
    delete d;
}


// public
kpThumbnailView *kpThumbnail::view () const
{
    return d->view;
}

// public
void kpThumbnail::setView (kpThumbnailView *view)
{
#if DEBUG_KP_THUMBNAIL
    kDebug () << "kpThumbnail::setView(" << view << ")";
#endif

    if (d->view == view)
        return;


    if (d->view)
    {
        disconnect (d->view, SIGNAL (destroyed ()),
                    this, SLOT (slotViewDestroyed ()));
        disconnect (d->view, SIGNAL (zoomLevelChanged (int, int)),
                    this, SLOT (updateCaption ()));

        d->lay->removeWidget (d->view);
    }

    d->view = view;

    if (d->view)
    {
        connect (d->view, SIGNAL (destroyed ()),
                 this, SLOT (slotViewDestroyed ()));
        connect (d->view, SIGNAL (zoomLevelChanged (int, int)),
                 this, SLOT (updateCaption ()));

        Q_ASSERT (d->view->parent () == this);
        d->lay->addWidget (d->view, Qt::AlignCenter);
        
        d->view->show ();
    }
    
    updateCaption ();
}


// public slot
void kpThumbnail::updateCaption ()
{
    setWindowTitle (view () ? view ()->caption () : i18nc ("@title:window", "Thumbnail"));
}


// protected slot
void kpThumbnail::slotViewDestroyed ()
{
#if DEBUG_KP_THUMBNAIL
    kDebug () << "kpThumbnail::slotViewDestroyed()";
#endif

    d->view = 0;
    updateCaption ();
}


// protected virtual [base QWidget]
void kpThumbnail::resizeEvent (QResizeEvent *e)
{
#if DEBUG_KP_THUMBNAIL
    kDebug () << "kpThumbnail::resizeEvent(" << width ()
               << "," << height () << ")" << endl;
#endif

    QWidget::resizeEvent (e);

    // updateVariableZoom ();  TODO: is below a good idea since this commented out?

    if (d->mainWindow)
    {
        d->mainWindow->notifyThumbnailGeometryChanged ();

        if (d->mainWindow->tool ())
            d->mainWindow->tool ()->somethingBelowTheCursorChanged ();
    }
}

// protected virtual [base QWidget]
void kpThumbnail::moveEvent (QMoveEvent * /*e*/)
{
    if (d->mainWindow)
        d->mainWindow->notifyThumbnailGeometryChanged ();
}

// protected virtual [base QWidget]
void kpThumbnail::closeEvent (QCloseEvent *e)
{
    QWidget::closeEvent (e);

    emit windowClosed ();
}
