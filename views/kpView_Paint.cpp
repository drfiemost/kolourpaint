
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


#define DEBUG_KP_VIEW 0
#define DEBUG_KP_VIEW_RENDERER ((DEBUG_KP_VIEW && 1) || 0)


#include "views/kpView.h"
#include "kpViewPrivate.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QScrollBar>

#include "kpLogCategories.h"

#include "layers/selections/kpAbstractSelection.h"
#include "imagelib/kpColor.h"
#include "document/kpDocument.h"
#include "layers/tempImage/kpTempImage.h"
#include "layers/selections/text/kpTextSelection.h"
#include "views/manager/kpViewManager.h"
#include "kpViewScrollableContainer.h"

//---------------------------------------------------------------------

// protected
QRect kpView::paintEventGetDocRect (const QRect &viewRect) const
{
    qCDebug(kpLogViews) << "kpView::paintEventGetDocRect(" << viewRect << ")";

    QRect docRect;

    // From the "we aren't sure whether to round up or round down" department:

    if (zoomLevelX () < 100 || zoomLevelY () < 100)
        docRect = transformViewToDoc (viewRect);
    else
    {
        // think of a grid - you need to fully cover the zoomed-in pixels
        // when docRect is zoomed back to the view later
        docRect = QRect (transformViewToDoc (viewRect.topLeft ()),  // round down
                         transformViewToDoc (viewRect.bottomRight ()));  // round down
    }

    if (zoomLevelX () % 100 || zoomLevelY () % 100)
    {
        // at least round up the bottom-right point and deal with matrix weirdness:
        // - helpful because it ensures we at least cover the required area
        //   at e.g. 67% or 573%
        docRect.setBottomRight (docRect.bottomRight () + QPoint (2, 2));
    }

    qCDebug(kpLogViews) << "\tdocRect=" << docRect;
    kpDocument *doc = document ();
    if (doc)
    {
        docRect = docRect.intersected (doc->rect ());
    #if DEBUG_KP_VIEW_RENDERER && 1
        qCDebug(kpLogViews) << "\tintersected with doc=" << docRect;
    #endif
    }

    return docRect;
}

//---------------------------------------------------------------------

// public static
void kpView::drawTransparentBackground (QPainter *painter,
                                        const QPoint &patternOrigin,
                                        const QRect &viewRect,
                                        bool isPreview)
{
    qCDebug(kpLogViews) << "kpView::drawTransparentBackground() patternOrigin="
              << patternOrigin
              << " viewRect=" << viewRect
              << " isPreview=" << isPreview;

    const int cellSize = !isPreview ? 16 : 10;

    // TODO: % is unpredictable with negatives.

    int starty = viewRect.y ();
    if ((starty - patternOrigin.y ()) % cellSize)
        starty -= ((starty - patternOrigin.y ()) % cellSize);

    int startx = viewRect.x ();
    if ((startx - patternOrigin.x ()) % cellSize)
        startx -= ((startx - patternOrigin.x ()) % cellSize);

    qCDebug(kpLogViews) << "\tstartXY=" << QPoint (startx, starty);

    painter->save ();

    // Clip to <viewRect> as we may draw outside it on all sides.
    painter->setClipRect (viewRect, Qt::IntersectClip/*honor existing clip*/);

    for (int y = starty; y <= viewRect.bottom (); y += cellSize)
    {
        for (int x = startx; x <= viewRect.right (); x += cellSize)
        {
            bool parity = ((x - patternOrigin.x ()) / cellSize +
                (y - patternOrigin.y ()) / cellSize) % 2;
            QColor col;

            if (parity)
            {
                if (!isPreview)
                    col = QColor (213, 213, 213);
                else
                    col = QColor (224, 224, 224);
            }
            else
                col = Qt::white;

            painter->fillRect (x, y, cellSize, cellSize, col);
        }
    }

    painter->restore ();
}

//---------------------------------------------------------------------

// protected
void kpView::paintEventDrawCheckerBoard (QPainter *painter, const QRect &viewRect)
{
    qCDebug(kpLogViews) << "kpView(" << objectName ()
               << ")::paintEventDrawCheckerBoard(viewRect=" << viewRect
               << ") origin=" << origin () << endl;

    kpDocument *doc = document ();
    if (!doc)
        return;

    QPoint patternOrigin = origin ();

    if (scrollableContainer ())
    {
        qCDebug(kpLogViews) << "\tscrollableContainer: contents[XY]="
                   << QPoint (scrollableContainer ()->horizontalScrollBar()->value (),
                              scrollableContainer ()->verticalScrollBar()->value ());
        // Make checkerboard appear static relative to the scroll view.
        // This makes it more obvious that any visible bits of the
        // checkboard represent transparent pixels and not gray and white
        // squares.
        patternOrigin = QPoint (scrollableContainer ()->horizontalScrollBar()->value(),
                                scrollableContainer ()->verticalScrollBar()->value());
        qCDebug(kpLogViews) << "\t\tpatternOrigin=" << patternOrigin;
    }

    // TODO: this static business doesn't work yet
    patternOrigin = QPoint (0, 0);

    drawTransparentBackground (painter, patternOrigin, viewRect);
}

//---------------------------------------------------------------------

// protected
void kpView::paintEventDrawSelection (QImage *destPixmap, const QRect &docRect)
{
    qCDebug(kpLogViews) << "kpView::paintEventDrawSelection() docRect=" << docRect;

    kpDocument *doc = document ();
    if (!doc)
    {
        qCDebug(kpLogViews) << "\tno doc - abort";
        return;
    }

    kpAbstractSelection *sel = doc->selection ();
    if (!sel)
    {
        qCDebug(kpLogViews) << "\tno sel - abort";
        return;
    }


    //
    // Draw selection pixmap (if there is one)
    //
    qCDebug(kpLogViews) << "\tdraw sel pixmap @ " << sel->topLeft ();
    sel->paint (destPixmap, docRect);


    //
    // Draw selection border
    //

    kpViewManager *vm = viewManager ();
    qCDebug(kpLogViews) << "\tsel border visible="
               << vm->selectionBorderVisible ()
               << endl;
    if (vm->selectionBorderVisible ())
    {
        sel->paintBorder (destPixmap, docRect, vm->selectionBorderFinished ());
    }


    //
    // Draw text cursor
    //

    // TODO: It would be nice to display the text cursor even if it's not
    //       within the text box (this can happen if the text box is too
    //       small for the text it contains).
    //
    //       However, too much selection repaint code assumes that it
    //       only paints inside its kpAbstractSelection::boundingRect().
    kpTextSelection *textSel = dynamic_cast <kpTextSelection *> (sel);
    if (textSel &&
        vm->textCursorEnabled () &&
        (vm->textCursorBlinkState () ||
        // For the current main window:
        //     As long as _any_ view has focus, blink _all_ views not just the
        //     one with focus.
        !vm->hasAViewWithFocus ()))  // sync: call will break when vm is not held by 1 mainWindow
    {
        QRect rect = vm->textCursorRect ();
        rect = rect.intersected (textSel->textAreaRect ());
        if (!rect.isEmpty ())
        {
          kpPixmapFX::fillRect(destPixmap,
              rect.x () - docRect.x (), rect.y () - docRect.y (),
              rect.width (), rect.height (),
              kpColor::LightGray, kpColor::DarkGray);
        }
    }
}

//---------------------------------------------------------------------

// protected
void kpView::paintEventDrawSelectionResizeHandles (const QRect &clipRect)
{
    qCDebug(kpLogViews) << "kpView::paintEventDrawSelectionResizeHandles("
               << clipRect << ")" << endl;

    if (!selectionLargeEnoughToHaveResizeHandles ())
    {
        qCDebug(kpLogViews) << "\tsel not large enough to have resize handles";
        return;
    }

    kpViewManager *vm = viewManager ();
    if (!vm || !vm->selectionBorderVisible () || !vm->selectionBorderFinished ())
    {
        qCDebug(kpLogViews) << "\tsel border not visible or not finished";
        return;
    }

    const QRect selViewRect = selectionViewRect ();
    qCDebug(kpLogViews) << "\tselViewRect=" << selViewRect;
    if (!selViewRect.intersects (clipRect))
    {
        qCDebug(kpLogViews) << "\tdoesn't intersect viewRect";
        return;
    }

    QRegion selResizeHandlesRegion = selectionResizeHandlesViewRegion (true/*for renderer*/);
    qCDebug(kpLogViews) << "\tsel resize handles view region="
               << selResizeHandlesRegion;

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::cyan);

    foreach (const QRect &r, selResizeHandlesRegion.rects())
      painter.drawRect(r);
}

//---------------------------------------------------------------------

// protected
void kpView::paintEventDrawTempImage (QImage *destPixmap, const QRect &docRect)
{
    kpViewManager *vm = viewManager ();
    if (!vm)
        return;

    const kpTempImage *tpi = vm->tempImage ();
    qCDebug(kpLogViews) << "kpView::paintEventDrawTempImage() tempImage="
               << tpi
               << " isVisible="
               << (tpi ? tpi->isVisible (vm) : false);

    if (!tpi || !tpi->isVisible (vm))
        return;

    tpi->paint (destPixmap, docRect);
}

//---------------------------------------------------------------------

// protected
void kpView::paintEventDrawGridLines (QPainter *painter, const QRect &viewRect)
{
  int hzoomMultiple = zoomLevelX () / 100;
  int vzoomMultiple = zoomLevelY () / 100;

  painter->setPen(Qt::gray);

  // horizontal lines
  int starty = viewRect.top();
  if (starty % vzoomMultiple)
    starty = (starty + vzoomMultiple) / vzoomMultiple * vzoomMultiple;

  for (int y = starty; y <= viewRect.bottom(); y += vzoomMultiple)
    painter->drawLine(viewRect.left(), y, viewRect.right(), y);

  // vertical lines
  int startx = viewRect.left();
  if (startx % hzoomMultiple)
    startx = (startx + hzoomMultiple) / hzoomMultiple * hzoomMultiple;

  for (int x = startx; x <= viewRect.right(); x += hzoomMultiple)
    painter->drawLine(x, viewRect.top (), x, viewRect.bottom());
}

//---------------------------------------------------------------------

// This is called "_Unclipped" because it may draw outside of
// <viewRect>.
//
// There are 2 reasons for doing so:
//
// A. If, for instance:
//
//    1. <viewRect> = QRect (0, 0, 2, 3) [top-left of the view]
//    2. zoomLevelX() == 800
//    3. zoomLevelY() == 800
//
//    Then, the local variable <docRect> will be QRect (0, 0, 1, 1).
//    When the part of the document corresponding to <docRect>
//    (a single document pixel) is drawn with QPainter::scale(), the
//    view rectangle QRect (0, 0, 7, 7) will be overwritten due to the
//    8x zoom.  This view rectangle is bigger than <viewRect>.
//
//    We can't use QPainter::setClipRect() since it is buggy in Qt 4.3.1
//    and clips too many pixels when used in combination with scale()
//    [qt-bugs@trolltech.com issue N181038].  ==> MK 10.2.2011 - fixed since Qt-4.4.4
//
// B. paintEventGetDocRect() may, by design, return a larger document
//    rectangle than what <viewRect> corresponds to, if the zoom levels
//    are not perfectly divisible by 100.
//
// This over-drawing is dangerous -- see the comments in paintEvent().
// This over-drawing is only safe from Qt's perspective since Qt
// automatically clips all drawing in paintEvent() (which calls us) to
// QPaintEvent::region().
void kpView::paintEventDrawDoc_Unclipped (const QRect &viewRect)
{
    QTime timer;
    timer.start ();
    qCDebug(kpLogViews) << "\tviewRect=" << viewRect;

    kpViewManager *vm = viewManager ();
    const kpDocument *doc = document ();

    Q_ASSERT (vm);
    Q_ASSERT (doc);

    if (viewRect.isEmpty ())
        return;

    QRect docRect = paintEventGetDocRect (viewRect);

    qCDebug(kpLogViews) << "\tdocRect=" << docRect;

    QPainter painter (this);
    //painter.setCompositionMode(QPainter::CompositionMode_Source);

    QImage docPixmap;
    bool tempImageWillBeRendered = false;

    // LOTODO: I think <docRect> being empty would be a bug.
    if (!docRect.isEmpty ())
    {
        docPixmap = doc->getImageAt (docRect);

        qCDebug(kpLogViews) << "\tdocPixmap.hasAlphaChannel()="
                  << docPixmap.hasAlphaChannel () << endl;

        tempImageWillBeRendered =
            (!doc->selection () &&
             vm->tempImage () &&
             vm->tempImage ()->isVisible (vm) &&
             docRect.intersects (vm->tempImage ()->rect ()));

        qCDebug(kpLogViews) << "\ttempImageWillBeRendered=" << tempImageWillBeRendered
                   << " (sel=" << doc->selection ()
                   << " tempImage=" << vm->tempImage ()
                   << " tempImage.isVisible=" << (vm->tempImage () ? vm->tempImage ()->isVisible (vm) : false)
                   << " docRect.intersects(tempImage.rect)=" << (vm->tempImage () ? docRect.intersects (vm->tempImage ()->rect ()) : false)
                   << ")";
    }


    //
    // Draw checkboard for transparent images and/or views with borders
    //

    if (docPixmap.hasAlphaChannel() ||
        (tempImageWillBeRendered && vm->tempImage ()->paintMayAddMask ()))
    {
        paintEventDrawCheckerBoard (&painter, viewRect);
    }

    if (!docRect.isEmpty ())
    {
        //
        // Draw docPixmap + tempImage
        //

        if (doc->selection ())
        {
            paintEventDrawSelection (&docPixmap, docRect);
        }
        else if (tempImageWillBeRendered)
        {
            paintEventDrawTempImage (&docPixmap, docRect);
        }

        qCDebug(kpLogViews) << "\torigin=" << origin ();
        // Blit scaled version of docPixmap + tempImage.
        QTime scaleTimer; scaleTimer.start ();
        // This is the only troublesome part of the method that draws unclipped.
        painter.translate (origin ().x (), origin ().y ());
        painter.scale (double (zoomLevelX ()) / 100.0,
                       double (zoomLevelY ()) / 100.0);
        painter.drawImage (docRect, docPixmap);
        //painter.resetMatrix ();  // back to 1-1 scaling
        qCDebug(kpLogViews) << "\tscale time=" << scaleTimer.elapsed ();

    }  // if (!docRect.isEmpty ()) {

    qCDebug(kpLogViews) << "\tdrawDocRect done in: " << timer.restart () << "ms";
}

//---------------------------------------------------------------------

// protected virtual [base QWidget]
void kpView::paintEvent (QPaintEvent *e)
{
    // sync: kpViewPrivate
    // WARNING: document(), viewManager() and friends might be 0 in this method.
    // TODO: I'm not 100% convinced that we always check if their friends are 0.

    QTime timer;
    timer.start ();

    kpViewManager *vm = viewManager ();

    qCDebug(kpLogViews) << "kpView(" << objectName () << ")::paintEvent() vm=" << (bool) vm
               << " queueUpdates=" << (vm && vm->queueUpdates ())
               << " fastUpdates=" << (vm && vm->fastUpdates ())
               << " viewRect=" << e->rect ()
               << " topLeft=" << QPoint (x (), y ());

    if (!vm)
        return;

    if (vm->queueUpdates ())
    {
        // OPT: if this update was due to the document,
        //      use document coordinates (in case of a zoom change in
        //      which view coordinates become out of date)
        addToQueuedArea (e->region ());
        return;
    }

    kpDocument *doc = document ();
    if (!doc)
        return;


    // It seems that e->region() is already clipped by Qt to the visible
    // part of the view (which could be quite small inside a scrollview).
    QRegion viewRegion = e->region ();
    QVector <QRect> rects = viewRegion.rects ();
    qCDebug(kpLogViews) << "\t#rects = " << rects.count ();

    // Draw all of the requested regions of the document _before_ drawing
    // the grid lines, buddy rectangle and selection resize handles.
    // This ordering is important since paintEventDrawDoc_Unclipped()
    // may draw outside of the view rectangle passed to it.
    //
    // To illustrate this, suppose we changed each iteration of the loop
    // to call paintEventDrawDoc_Unclipped() _and_ then,
    // paintEventDrawGridLines().  If there are 2 or more iterations of this
    // loop, paintEventDrawDoc_Unclipped() in one iteration may draw over
    // parts of nearby grid lines (which were drawn in a previous iteration)
    // with document pixels.  Those grid line parts are probably not going to
    // be redrawn, so will appear to be missing.
    foreach (const QRect &r, rects)
    {
        paintEventDrawDoc_Unclipped (r);
    }

    //
    // Draw Grid Lines
    //

    if ( isGridShown() )
    {
      QPainter painter(this);
      foreach (const QRect &r, rects)
        paintEventDrawGridLines(&painter, r);
    }

    const QRect r = buddyViewScrollableContainerRectangle();
    if ( !r.isEmpty() )
    {
      QPainter painter(this);

      painter.setPen(QPen(Qt::lightGray, 1/*width*/, Qt::DotLine));
      painter.setBackground(Qt::darkGray);
      painter.setBackgroundMode(Qt::OpaqueMode);

      painter.drawRect(r.x(), r.y(), r.width() - 1, r.height() - 1);
    }

    if (doc->selection ())
    {
        // Draw resize handles on top of possible grid lines
        paintEventDrawSelectionResizeHandles (e->rect ());
    }

    qCDebug(kpLogViews) << "\tall done in: " << timer.restart () << "ms";
}
