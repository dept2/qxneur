#ifndef QXNMENU_H
#define QXNMENU_H

#include <QMenu>

class QAction;

class QXNMenu : public QMenu
{
  Q_OBJECT
  public:
    QXNMenu(QWidget* parent = 0);
    ~QXNMenu();

  signals:
    void settingsRequested();
    void applicationQuit();
};

#endif // QXNMENU_H
