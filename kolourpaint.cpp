
/*
   Copyright (c) 2003-2006 Clarence Dang <dang@kde.org>
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


#include <qfile.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <kimageio.h>
#include <klocale.h>

#include <kpdefs.h>
#include <kpmainwindow.h>

#include <kolourpaintlicense.h>
#include <kolourpaintversion.h>


static const KCmdLineOptions cmdLineOptions [] =
{
    {"+[file]", I18N_NOOP ("Image file to open"), 0},
    KCmdLineLastOption
};


int main (int argc, char *argv [])
{
    KAboutData aboutData
    (
        "kolourpaint",
        I18N_NOOP ("KolourPaint"),
        kpVersionText,
        I18N_NOOP ("Paint Program for KDE"),
        KAboutData::License_Custom,
        0/*copyright statement - see license instead*/,
        I18N_NOOP ("Support / Feedback:\n"
                   "kolourpaint-support@lists.sourceforge.net\n"),
        "http://www.kolourpaint.org/"
    );


    // this is _not_ the same as KAboutData::License_BSD
    aboutData.setLicenseText (kpLicenseText);


    aboutData.setCustomAuthorText (
        I18N_NOOP
        (
            "\n"
            "For support, or to report bugs and feature requests, please email\n"
            "<kolourpaint-support@lists.sourceforge.net>"
            " - the free and friendly\n"
            "KolourPaint support service.\n"
            "\n"
        ),
        I18N_NOOP
        (
            "<qt>"
            "For support, or to report bugs and feature requests, please email<br>"
            "<a href=\"mailto:kolourpaint-support@lists.sourceforge.net\">kolourpaint-support@lists.sourceforge.net</a>"
            " - the free and friendly<br>"
            "KolourPaint support service.<br>"
            "<br>"
            "</qt>"
        ));


    // SYNC: with AUTHORS

    aboutData.addAuthor ("Clarence Dang", I18N_NOOP ("Maintainer"), "dang@kde.org");
    aboutData.addAuthor ("Thurston Dang", I18N_NOOP ("Chief Investigator"),
                         "thurston_dang@users.sourceforge.net");
    aboutData.addAuthor ("Kristof Borrey", I18N_NOOP ("Icons"), "borrey@kde.org");
    aboutData.addAuthor ("Kazuki Ohta", I18N_NOOP ("InputMethod Support"), "mover@hct.zaq.ne.jp");
    aboutData.addAuthor ("Nuno Pinheiro", I18N_NOOP ("Icons"), "nf.pinheiro@gmail.com");
    aboutData.addAuthor ("Danny Allen", I18N_NOOP ("Icons"), "dannya40uk@yahoo.co.uk");
    aboutData.addAuthor ("Laurent Montel", I18N_NOOP ("KDE 4 Porting"), "montel@kde.org");


    aboutData.addCredit ("Rashid N. Achilov");
    aboutData.addCredit ("Toyohiro Asukai");
    aboutData.addCredit ("Bela-Andreas Bargel");
    aboutData.addCredit ("Waldo Bastian");
    aboutData.addCredit ("Ismail Belhachmi");
    aboutData.addCredit ("Sashmit Bhaduri");
    aboutData.addCredit ("Antonio Bianco");
    aboutData.addCredit ("Stephan Binner");
    aboutData.addCredit ("Markus Brueffer");
    aboutData.addCredit ("Rob Buis");
    aboutData.addCredit ("Lucijan Busch");
    aboutData.addCredit ("Mikhail Capone");
    aboutData.addCredit ("Enrico Ceppi");
    aboutData.addCredit ("Tom Chance");
    aboutData.addCredit ("Albert Astals Cid");
    aboutData.addCredit ("Jennifer Dang");
    aboutData.addCredit ("Lawrence Dang");
    aboutData.addCredit ("Christoph Eckert");
    aboutData.addCredit ("David Faure");
    aboutData.addCredit ("P. Fisher");
    aboutData.addCredit ("Nicolas Goutte");
    aboutData.addCredit ("Herbert Graeber");
    aboutData.addCredit ("Brad Grant");
    aboutData.addCredit ("David Greenaway");
    aboutData.addCredit ("Wilco Greven");
    aboutData.addCredit ("Hubert Grininger");
    aboutData.addCredit ("Adriaan de Groot");
    aboutData.addCredit ("Esben Mose Hansen");
    aboutData.addCredit ("Nadeem Hasan");
    aboutData.addCredit ("Simon Hausmann");
    aboutData.addCredit ("Michael Hoehne");
    aboutData.addCredit ("Andrew J");
    aboutData.addCredit ("Werner Joss");
    aboutData.addCredit ("Derek Kite");
    aboutData.addCredit ("Tobias Koenig");
    aboutData.addCredit ("Dmitry Kolesnikov");
    aboutData.addCredit ("Stephan Kulow");
    aboutData.addCredit ("Eric Laffoon");
    aboutData.addCredit ("Michael Lake");
    aboutData.addCredit ("Sebastien Laout");
    aboutData.addCredit ("David Ling");
    aboutData.addCredit ("Volker Lochte");
    aboutData.addCredit ("Anders Lund");
    aboutData.addCredit ("Thiago Macieira");
    aboutData.addCredit ("Jacek Masiulaniec");
    aboutData.addCredit ("Benjamin Meyer");
    aboutData.addCredit ("Amir Michail");
    aboutData.addCredit ("Robert Moszczynski");
    aboutData.addCredit ("Dirk Mueller");
    aboutData.addCredit ("Ruivaldo Neto");
    aboutData.addCredit ("Ralf Nolden");
    aboutData.addCredit ("Maks Orlovich");
    aboutData.addCredit ("Steven Pasternak");
    aboutData.addCredit ("Cédric Pasteur");
    aboutData.addCredit ("Erik K. Pedersen");
    aboutData.addCredit ("Dennis Pennekamp");
    aboutData.addCredit ("Jos Poortvliet");
    aboutData.addCredit ("Boudewijn Rempt");
    aboutData.addCredit ("Marcos Rodriguez");
    aboutData.addCredit ("Matt Rogers");
    aboutData.addCredit ("Francisco Jose Canizares Santofimia");
    aboutData.addCredit ("Bram Schoenmakers");
    aboutData.addCredit ("Dirk Schönberger");
    aboutData.addCredit ("Lutz Schweizer");
    aboutData.addCredit ("Emmeran Seehuber");
    aboutData.addCredit ("Peter Simonsson");
    aboutData.addCredit ("Andrew Simpson");
    aboutData.addCredit ("A T Somers");
    aboutData.addCredit ("Igor Stepin");
    aboutData.addCredit ("Stephen Sweeney");
    aboutData.addCredit ("Bart Symons");
    aboutData.addCredit ("Stefan Taferner");
    aboutData.addCredit ("Hogne Titlestad");
    aboutData.addCredit ("Brandon Mark Turner");
    aboutData.addCredit ("Jonathan Turner");
    aboutData.addCredit ("Stephan Unknown");
    aboutData.addCredit ("Dries Verachtert");
    aboutData.addCredit ("Simon Vermeersch");
    aboutData.addCredit ("Lauri Watts");
    aboutData.addCredit ("Mark Wege");
    aboutData.addCredit ("Christoph Wiesen");
    aboutData.addCredit ("Andre Wobbeking");
    aboutData.addCredit ("Luke-Jr");
    aboutData.addCredit ("Maxim_86ualb2");
    aboutData.addCredit ("Michele");


    KCmdLineArgs::init (argc, argv, &aboutData);
    KCmdLineArgs::addCmdLineOptions (cmdLineOptions);

    KApplication app;

    // Qt says this is necessary but I don't think it is...
    QObject::connect (&app, SIGNAL (lastWindowClosed ()),
                      &app, SLOT (quit ()));


    if (app.isSessionRestored ())
        RESTORE (kpMainWindow)
    else
    {
        kpMainWindow *mainWindow;
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs ();

        if (args->count () >= 1)
        {
            for (int i = 0; i < args->count (); i++)
            {
                mainWindow = new kpMainWindow (args->url (i));
                mainWindow->show ();
            }
        }
        else
        {
            mainWindow = new kpMainWindow ();
            mainWindow->show ();
        }

        args->clear ();
    }


    return app.exec ();
}
