
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

#define DEBUG_KP_TOOL_TEXT 0


#include <kpToolText.h>
#include <kpToolTextPrivate.h>

#include <qevent.h>
#include <qlist.h>

#include <kdebug.h>
#include <klocale.h>

#include <kpCommandHistory.h>
#include <kpDocument.h>
#include <kpTextSelection.h>
#include <kpToolTextBackspaceCommand.h>
#include <kpToolTextChangeStyleCommand.h>
#include <kpToolSelectionCreateCommand.h>
#include <kpToolSelectionEnvironment.h>
#include <kpToolTextDeleteCommand.h>
#include <kpToolTextEnterCommand.h>
#include <kpToolTextInsertCommand.h>
#include <kpToolWidgetOpaqueOrTransparent.h>
#include <kpView.h>
#include <kpViewManager.h>


// protected virtual [base kpTool]
bool kpToolText::viewEvent (QEvent *e)
{
    const bool isShortcutOverrideEvent =
        (e->type () == QEvent::ShortcutOverride);
    const bool haveTextSelection = document ()->textSelection ();

#if DEBUG_KP_TOOL_TEXT && 0
    kDebug () << "kpToolText::viewEvent() type=" << e->type ()
              << " isShortcutOverrideEvent=" << isShortcutOverrideEvent
              << " haveTextSel=" << haveTextSelection
              << endl;
#endif

    if (!isShortcutOverrideEvent || !haveTextSelection)
        return kpAbstractSelectionTool::viewEvent (e);

    QKeyEvent *ke = static_cast <QKeyEvent *> (e);
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::viewEvent() key=" << ke->key ()
              << " modifiers=" << ke->modifiers ()
              << " QChar.isPrint()=" << QChar (ke->key ()).isPrint ()
              << endl;
#endif

    // Can't be shortcut?
    if (ke->key () == 0 && ke->key () == Qt::Key_unknown)
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\tcan't be shortcut - safe to not react";
    #endif
    }
    // Normal letter (w/ or w/o shift, keypad button ok)?
    // TODO: don't like this check
    else if ((ke->modifiers () &
                (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier)) == 0 &&
             !( ke->text ().isEmpty ()))
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\tis text - grab";
    #endif
        e->accept ();
    }
    else
    {
        // Strickly speaking, we should grab stuff like the arrow keys
        // and enter.  In any case, should be done down in kpTool (as that
        // uses arrow keys too).
    }

    return kpAbstractSelectionTool::event (e);
}


// protected virtual [base kpAbstractSelectionTool]
void kpToolText::keyPressEvent (QKeyEvent *e)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::keyPressEvent(e->text='" << e->text () << "')";
#endif


    e->ignore ();


    if (hasBegunDraw ())
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\talready began draw with mouse - passing on event to kpTool";
    #endif
        kpAbstractSelectionTool::keyPressEvent (e);
        return;
    }


    kpTextSelection * const textSel = document ()->textSelection ();

    if (!textSel)
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\tno text sel - passing on event to kpTool";
    #endif
        //if (hasBegunShape ())
        //    endShape (currentPoint (), normalizedRect ());

        kpAbstractSelectionTool::keyPressEvent (e);
        return;
    }


    // (All handle.+()'s require this info)
    const QList <QString> textLines = textSel->textLines ();
    const int cursorRow = viewManager ()->textCursorRow ();
    const int cursorCol = viewManager ()->textCursorCol ();


    // TODO: KTextEdit::keyPressEvent() uses KStandardShortcut instead of hardcoding; same fix for kpTool?
    switch (e->key ())
    {
    case Qt::Key_Up:
        handleUpKeyPress (e, textLines, cursorRow, cursorCol);
        break;

    case Qt::Key_Down:
        handleDownKeyPress (e, textLines, cursorRow, cursorCol);
        break;

    case Qt::Key_Left:
        handleLeftKeyPress (e, textLines, cursorRow, cursorCol);
        break;

    case Qt::Key_Right:
        handleRightKeyPress (e, textLines, cursorRow, cursorCol);
        break;


    case Qt::Key_Home:
        handleHomeKeyPress (e, textLines, cursorRow, cursorCol);
        break;

    case Qt::Key_End:
        handleEndKeyPress (e, textLines, cursorRow, cursorCol);
        break;


    case Qt::Key_Backspace:
        handleBackspaceKeyPress (e, textLines, cursorRow, cursorCol);
        break;

    case Qt::Key_Delete:
        handleDeleteKeyPress (e, textLines, cursorRow, cursorCol);
        break;


    case Qt::Key_Enter:
    case Qt::Key_Return:
        handleEnterKeyPress (e, textLines, cursorRow, cursorCol);
        break;


    default:
        handleTextTyped (e, textLines, cursorRow, cursorCol);
        break;
    }


    if (!e->isAccepted ())
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\tkey processing did not accept (text was '"
                   << e->text ()
                   << "') - passing on event to kpAbstractSelectionTool"
                   << endl;
    #endif
        //if (hasBegunShape ())
        //    endShape (currentPoint (), normalizedRect ());

        kpAbstractSelectionTool::keyPressEvent (e);
        return;
    }
}
