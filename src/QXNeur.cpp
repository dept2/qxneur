// C++ Implementation: QXNeur
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009

// Local
#include "QXNeur.h"
#include "QXNKeyboard.h"
#include "QXNTrayIcon.h"

// Qt
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>


QXNeur::QXNeur(int argc, char** argv)
  : QApplication(argc, argv)
{
  // Tell QApplication not to close when the configuration dialog is closed
  setQuitOnLastWindowClosed(false);

  // Create and install the Qt translator
  QTranslator* qtTranslator = new QTranslator(this);
  qtTranslator->load(QString(QLatin1String("qt_%1")).arg(QLocale::system().name()),
                     QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  installTranslator(qtTranslator);

  // Create and install the application translator
  QTranslator* appTranslator = new QTranslator(this);
  appTranslator->load(QString(QLatin1String("qxneur_%1")).arg(QLocale::system().name()),
                      QString(QLatin1String("%1/share")).arg(QLatin1String(INSTALL_PREFIX)));
  installTranslator(appTranslator);

  // Initialize keyboard object
  keyboard = new QXNKeyboard(this);

  // Initialize the tray icon
  trayIcon = new QXNTrayIcon(keyboard, this);

  trayIcon->show();
}


QXNeur::~QXNeur()
{
  delete trayIcon;
}


bool QXNeur::x11EventFilter(XEvent* event)
{
  keyboard->x11Event(event);
  return false;
}
