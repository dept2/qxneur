#ifndef QXNMENU_H
#define QXNMENU_H

#include <QMenu>

class QXNConfig;

class QAction;

class QXNMenu : public QMenu
{
  Q_OBJECT
  public:
    QXNMenu(QXNConfig* xnconfig, QWidget* parent = 0);
    ~QXNMenu();

  signals:
    void settingsRequested();
    void applicationQuit();
};

#endif // QXNMENU_H
