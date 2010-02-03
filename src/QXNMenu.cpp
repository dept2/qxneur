#include <QApplication>
#include <QAction>

#include "QXNMenu.h"
#include "QXNConfig.h"

QXNMenu::QXNMenu(QXNConfig* xnconfig, QWidget* parent)
  : QMenu(parent)
{
  // Manual mode action
  QAction* manualAction = new QAction(tr("Manual mode"), this);
  manualAction->setCheckable(true);
  manualAction->setChecked(xnconfig->manualMode());
  addAction(manualAction);
  connect(manualAction, SIGNAL(toggled(bool)), xnconfig, SLOT(setManualModeNow(bool)));

  // Settings action
  addAction(QIcon(QLatin1String(":/actions/settings.png")), tr("Settings..."),
            this, SIGNAL(settingsRequested()));
  addSeparator();

  // Quit action
  addAction(QIcon(QLatin1String(":/actions/quit.png")), tr("Quit"),
            this, SIGNAL(applicationQuit()));
}

QXNMenu::~QXNMenu()
{
}
