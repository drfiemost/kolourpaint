
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


#define DEBUG_KP_THUMBNAIL_VIEW 0


#include <kpthumbnailview.h>

#include <kdebug.h>


kpThumbnailView::kpThumbnailView (kpDocument *document,
        kpToolToolBar *toolToolBar,
        kpViewManager *viewManager,
        kpView *buddyView,
        kpViewScrollableContainer *buddyViewScrollView,
        QWidget *parent, const char *name)

    : kpView (document, toolToolBar, viewManager,
              buddyView, buddyViewScrollView,
              parent, name)
{
}

kpThumbnailView::~kpThumbnailView ()
{
}


// protected
void kpThumbnailView::setMaskToCoverDocument ()
{
#if DEBUG_KP_THUMBNAIL_VIEW
    kdDebug () << "kpThumbnailView::setMaskToCoverDocument()"
               << " origin=" << origin ()
               << " zoomedDoc: width=" << zoomedDocWidth ()
               << " height=" << zoomedDocHeight ()
               << endl;
#endif

    setMask (QRegion (QRect (origin ().x (), origin ().y (),
                      zoomedDocWidth (), zoomedDocHeight ())));
}


// protected virtual [base kpView]
void kpThumbnailView::resizeEvent (QResizeEvent *e)
{
#if DEBUG_KP_THUMBNAIL_VIEW
    kdDebug () << "kpThumbnailView(" << name () << ")::resizeEvent()"
               << endl;
#endif

    // For QResizeEvent's, Qt already throws an entire widget repaint into
    // the event loop.  So eat useless update() calls that can only slow
    // things down.
    // TODO: this doesn't seem to work.
    const bool oldIsUpdatesEnabled = isUpdatesEnabled ();
    setUpdatesEnabled (false);

    {
        kpView::resizeEvent (e);

        adjustToEnvironment ();
    }

    setUpdatesEnabled (oldIsUpdatesEnabled);
}


#include <kpthumbnailview.moc>

