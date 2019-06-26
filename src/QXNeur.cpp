// C++ Implementation: QXNeur
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009

// Local
#include "QXNeur.h"
#include "QXNTrayIcon.h"
#include "KbdLayout.h"
#include "KbdInfo.h"

// Qt
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSettings>

#include <xcb/xcb.h>


QXNeur::QXNeur(int argc, char** argv)
  : QApplication(argc, argv)
{
  // Tell QApplication not to close when the configuration dialog is closed
  setQuitOnLastWindowClosed(false);

  // Set QApplication properties to load config
  setOrganizationName(QLatin1String("cyberbobs"));
  setOrganizationDomain(QLatin1String("https://github.com/cyberbobs"));
  setApplicationName(QLatin1String("QXLayoutIndicator"));

  // Settings object
  settings = new QSettings(this);

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
  keyboard = new X11Kbd;
  keyboard->init();
  keyboard->readKbdInfo();

  QString defaultLayout = settings->value(QLatin1String("DefaultLayout")).toString();
  if (!defaultLayout.isEmpty())
  {
    QXNLanguage::Language language = QXNLanguage::layoutNameToLanguage(defaultLayout);
    if (language != QXNLanguage::Unknown)
      keyboard->setGroup(defaultLayout);
  }

  // Initialize the tray icon
  trayIcon = new QXNTrayIcon(keyboard, settings, this);
  trayIcon->show();
}


QXNeur::~QXNeur()
{
  delete trayIcon;
  delete keyboard;
}
