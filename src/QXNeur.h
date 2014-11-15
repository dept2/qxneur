#ifndef QXNEUR_H
#define QXNEUR_H

// Qt
#include <QApplication>
class QSettings;

// Local
class QXNKeyboard;
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
    QSettings* settings;
};

#endif // QXNEUR_H
