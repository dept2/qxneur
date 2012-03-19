#ifndef QXNEUR_H
#define QXNEUR_H

// Qt
#include <QApplication>

// Local
class QXNKeyboard;
class QXNMenu;
class QXNTrayIcon;

class QXNeur : public QApplication
{
  Q_OBJECT

  public:
    QXNeur(int argc, char** argv);
    ~QXNeur();

    bool x11EventFilter(XEvent* event);

  private:
    QXNKeyboard* keyboard;
    QXNTrayIcon* trayIcon;
    QXNMenu* trayMenu;
};

#endif // QXNEUR_H
