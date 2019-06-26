#ifndef QXNEUR_H
#define QXNEUR_H

// Qt
#include <QtGui>
#include <QApplication>
class QSettings;

// Local
class QXNTrayIcon;
class X11Kbd;


class QXNeur : public QApplication
{
  Q_OBJECT

  public:
    QXNeur(int argc, char** argv);
    ~QXNeur();

  private:
    X11Kbd* keyboard;
    QXNTrayIcon* trayIcon;
    QSettings* settings;
};

#endif // QXNEUR_H
