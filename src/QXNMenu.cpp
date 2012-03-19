// Qt
#include <QApplication>
#include <QAction>

// Local
#include "QXNMenu.h"


QXNMenu::QXNMenu(QWidget* parent)
  : QMenu(parent)
{
  // Quit action
  addAction(QIcon(QLatin1String(":/actions/quit.png")), tr("Quit"),
            this, SIGNAL(applicationQuit()));
}


QXNMenu::~QXNMenu()
{}
