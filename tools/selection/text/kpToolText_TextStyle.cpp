
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
#include <kpToolTextGiveContentCommand.h>
#include <kpToolSelectionCreateCommand.h>
#include <kpToolSelectionEnvironment.h>
#include <kpToolTextDeleteCommand.h>
#include <kpToolTextEnterCommand.h>
#include <kpToolTextInsertCommand.h>
#include <kpToolWidgetOpaqueOrTransparent.h>
#include <kpView.h>
#include <kpViewManager.h>


// protected
bool kpToolText::shouldChangeTextStyle () const
{
    if (environ ()->settingTextStyle ())
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\trecursion - abort setting text style: "
                   << environ ()->settingTextStyle ()
                   << endl;
    #endif
        return false;
    }

    if (!document ()->textSelection ())
    {
    #if DEBUG_KP_TOOL_TEXT
        kDebug () << "\tno text selection - abort setting text style";
    #endif
        return false;
    }

    return true;
}

// protected
void kpToolText::changeTextStyle (const QString &name,
                                  const kpTextStyle &newTextStyle,
                                  const kpTextStyle &oldTextStyle)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::changeTextStyle(" << name << ")";
#endif

    if (hasBegunShape ())
        endShape (currentPoint (), normalizedRect ());

    commandHistory ()->addCommand (
        new kpToolTextChangeStyleCommand (
            name,
            newTextStyle,
            oldTextStyle,
            environ ()->commandEnvironment ()));
}


// protected slot virtual [base kpAbstractSelectionTool]
void kpToolText::slotIsOpaqueChanged (bool isOpaque)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotIsOpaqueChanged()";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setBackgroundOpaque (!isOpaque);

    changeTextStyle (newTextStyle.isBackgroundOpaque () ?
                         i18n ("Text: Opaque Background") :
                         i18n ("Text: Transparent Background"),
                     newTextStyle,
                     oldTextStyle);
}

// protected slot virtual [base kpTool]
void kpToolText::slotColorsSwapped (const kpColor &newForegroundColor,
                                    const kpColor &newBackgroundColor)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotColorsSwapped()";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setForegroundColor (newBackgroundColor);
    oldTextStyle.setBackgroundColor (newForegroundColor);

    changeTextStyle (i18n ("Text: Swap Colors"),
                     newTextStyle,
                     oldTextStyle);
}

// protected slot virtual [base kpTool]
void kpToolText::slotForegroundColorChanged (const kpColor & /*color*/)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotForegroundColorChanged()";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setForegroundColor (oldForegroundColor ());

    changeTextStyle (i18n ("Text: Foreground Color"),
                     newTextStyle,
                     oldTextStyle);
}

// protected slot virtual [base kpAbstractSelectionTool]
void kpToolText::slotBackgroundColorChanged (const kpColor & /*color*/)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotBackgroundColorChanged()";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setBackgroundColor (oldBackgroundColor ());

    changeTextStyle (i18n ("Text: Background Color"),
                     newTextStyle,
                     oldTextStyle);
}

// protected slot virtual [base kpAbstractSelectionTool]
void kpToolText::slotColorSimilarityChanged (double, int)
{
    // --- don't pass on event to kpAbstractSelectionTool which would have set the
    //     SelectionTransparency - not relevant to the Text Tool ---
}


// public slot
void kpToolText::slotFontFamilyChanged (const QString &fontFamily,
                                        const QString &oldFontFamily)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotFontFamilyChanged() new="
               << fontFamily
               << " old="
               << oldFontFamily
               << endl;
#else
    (void) fontFamily;
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setFontFamily (oldFontFamily);

    changeTextStyle (i18n ("Text: Font"),
                     newTextStyle,
                     oldTextStyle);
}

// public slot
void kpToolText::slotFontSizeChanged (int fontSize, int oldFontSize)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotFontSizeChanged() new="
               << fontSize
               << " old="
               << oldFontSize
               << endl;
#else
    (void) fontSize;
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setFontSize (oldFontSize);

    changeTextStyle (i18n ("Text: Font Size"),
                     newTextStyle,
                     oldTextStyle);
}


// public slot
void kpToolText::slotBoldChanged (bool isBold)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotBoldChanged(" << isBold << ")";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setBold (!isBold);

    changeTextStyle (i18n ("Text: Bold"),
                     newTextStyle,
                     oldTextStyle);
}

// public slot
void kpToolText::slotItalicChanged (bool isItalic)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotItalicChanged(" << isItalic << ")";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setItalic (!isItalic);

    changeTextStyle (i18n ("Text: Italic"),
                     newTextStyle,
                     oldTextStyle);
}

// public slot
void kpToolText::slotUnderlineChanged (bool isUnderline)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotUnderlineChanged(" << isUnderline << ")";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setUnderline (!isUnderline);

    changeTextStyle (i18n ("Text: Underline"),
                     newTextStyle,
                     oldTextStyle);
}

// public slot
void kpToolText::slotStrikeThruChanged (bool isStrikeThru)
{
#if DEBUG_KP_TOOL_TEXT
    kDebug () << "kpToolText::slotStrikeThruChanged(" << isStrikeThru << ")";
#endif

    if (!shouldChangeTextStyle ())
        return;

    kpTextStyle newTextStyle = environ ()->textStyle ();

    // Figure out old text style.
    kpTextStyle oldTextStyle = newTextStyle;
    oldTextStyle.setStrikeThru (!isStrikeThru);

    changeTextStyle (i18n ("Text: Strike Through"),
                     newTextStyle,
                     oldTextStyle);
}
