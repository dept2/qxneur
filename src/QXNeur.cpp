// C++ Implementation: QXNeur
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009

// Local
#include "QXNeur.h"
#include "QXNKeyboard.h"
#include "QXNMenu.h"
#include "QXNTrayIcon.h"
#include "QXNConfig.h"
#include "QXNConfigDialog.h"

// Qt
#include <QProcess>
#include <QTranslator>
#include <QLibraryInfo>


QXNeur::QXNeur(int& argc, char** argv)
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
  appTranslator->load(QString(QLatin1String("qxneur_%1")).arg(QLocale::system().name()));
  installTranslator(appTranslator);

  // Initialize keyboard object
  keyboard = new QXNKeyboard(this);

  // Initialize the tray icon
  trayIcon = new QXNTrayIcon(keyboard, this);

  // Initialize the QProcess for xneur running
  xneur = new QProcess(this);
  xneur->start(QLatin1String("xneur"));
  if (!xneur->waitForStarted(3000))
    qFatal("%s", qPrintable(tr("Can not start xneur (unknown error)")));

  // Initialize the configuration dialog
  xnconfig = new QXNConfig(this);
  configDialog = new QXNConfigDialog(xnconfig);

  // Initialize the tray icon context menu
  trayMenu = new QXNMenu(xnconfig);

  // Connect the interoperation signals
  connect(trayIcon, SIGNAL(doubleClicked()), configDialog, SLOT(run()));
  connect(trayMenu, SIGNAL(settingsRequested()), configDialog, SLOT(run()));
  connect(trayMenu, SIGNAL(applicationQuit()), SLOT(quit()));

  trayIcon->setContextMenu(trayMenu);
  trayIcon->show();
}

QXNeur::~QXNeur()
{
  delete configDialog;

  delete trayMenu;

  // TODO: implement the proper stopping of xneur
  xneur->close();
  delete xneur;

  delete trayIcon;
  delete keyboard;
}


bool QXNeur::x11EventFilter(XEvent* event)
{
  keyboard->x11Event(event);
  return false;
}
