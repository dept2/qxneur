// C++ Interface: QXNeur
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009
#ifndef QXNEUR_H
#define QXNEUR_H

// Qt
#include <QApplication>
class QProcess;

// Local
class QXNKeyboard;
class QXNMenu;
class QXNTrayIcon;
class QXNConfig;
class QXNConfigDialog;

class QXNeur : public QApplication
{
  Q_OBJECT

  public:
    QXNeur(int& argc, char** argv);
    ~QXNeur();

    bool x11EventFilter(XEvent* event);

  private:
    QXNKeyboard* keyboard;
    QXNTrayIcon* trayIcon;
    QXNMenu* trayMenu;
    QProcess* xneur;
    QXNConfig* xnconfig;
    QXNConfigDialog* configDialog;
};

#endif // QXNEUR_H
