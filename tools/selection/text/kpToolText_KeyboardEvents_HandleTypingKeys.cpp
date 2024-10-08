
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


//---------------------------------------------------------------------

// protected
void kpToolText::handleBackspaceKeyPress (QKeyEvent *e,
    const QList <QString> &textLines, int cursorRow, int cursorCol)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "\tbackspace pressed";
#endif

    if (!textLines.isEmpty ())
    {
        if ((e->modifiers () & Qt::ControlModifier) == 0)
        {
            if (!d->backspaceCommand)
                addNewBackspaceCommand (&d->backspaceCommand);
    
            d->backspaceCommand->addBackspace ();
        }
        else
        {
            if (!d->backspaceWordCommand)
                addNewBackspaceCommand (&d->backspaceWordCommand);
    
            const int numMoves = MoveCursorToWordStart (textLines,
                &cursorRow, &cursorCol);
    
            viewManager ()->setQueueUpdates ();
            {
                for (int i = 0; i < numMoves; i++)
                    d->backspaceWordCommand->addBackspace ();
            }
            viewManager ()->restoreQueueUpdates ();
    
            Q_ASSERT (cursorRow == viewManager ()->textCursorRow ());
            Q_ASSERT (cursorCol == viewManager ()->textCursorCol ());
        }
    }

    e->accept ();
}

//---------------------------------------------------------------------

// protected
void kpToolText::handleDeleteKeyPress (QKeyEvent *e,
    const QList <QString> & textLines, int cursorRow, int cursorCol)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "\tdelete pressed";
#endif

    if (!textLines.isEmpty ())
    {
        if ((e->modifiers () & Qt::ControlModifier) == 0)
        {
            if (!d->deleteCommand)
                addNewDeleteCommand (&d->deleteCommand);
    
            d->deleteCommand->addDelete ();
        }
        else
        {
            if (!d->deleteWordCommand)
                addNewDeleteCommand (&d->deleteWordCommand);
    
            // We don't want to know the cursor pos of the next word start
            // as delete should keep cursor in same pos.
            int cursorRowThrowAway = cursorRow,
                cursorColThrowAway = cursorCol;
            const int numMoves = MoveCursorToNextWordStart (textLines,
                &cursorRowThrowAway, &cursorColThrowAway);
    
            viewManager ()->setQueueUpdates ();
            {
                for (int i = 0; i < numMoves; i++)
                    d->deleteWordCommand->addDelete ();
            }
            viewManager ()->restoreQueueUpdates ();
    
            // Assert unchanged as delete should keep cursor in same pos.
            Q_ASSERT (cursorRow == viewManager ()->textCursorRow ());
            Q_ASSERT (cursorCol == viewManager ()->textCursorCol ());
        }
    }

    e->accept ();
}

//---------------------------------------------------------------------

// protected
void kpToolText::handleEnterKeyPress (QKeyEvent *e,
    const QList <QString> & /*textLines*/, int /*cursorRow*/, int /*cursorCol*/)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "\tenter pressed";
#endif

    // It's OK for <textLines> to be empty.
    
    if (!d->enterCommand)
        addNewEnterCommand (&d->enterCommand);

    d->enterCommand->addEnter ();

    e->accept ();
}

//---------------------------------------------------------------------

// protected
void kpToolText::handleTextTyped (QKeyEvent *e,
    const QList <QString> & /*textLines*/, int /*cursorRow*/, int /*cursorCol*/)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "\ttext=" << e->text();
#endif
    QString usableText;
    for (int i = 0; i < (int) e->text ().length (); i++)
    {
        if (e->text ().at (i).isPrint ())
            usableText += e->text ().at (i);
    }
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "\tusableText=" << usableText;
#endif

    if (usableText.isEmpty ())
    {
        // Don't end the current shape nor accept the event -- the event
        // wasn't for us.
        return;
    }

    // --- It's OK for <textLines> to be empty. ---

    if (!d->insertCommand)
        addNewInsertCommand (&d->insertCommand);

    d->insertCommand->addText (usableText);

    e->accept ();
}
