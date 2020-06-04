// Local
#include "QXNTrayIcon.h"
#include "KbdLayout.h"
#include "KbdInfo.h"

// Qt
#include <QMenu>
#include <QSettings>
#include <QApplication>
#include <QDebug>


QXNTrayIcon::QXNTrayIcon(X11Kbd* keyboard, QSettings* settings, QObject* parent)
  : QSystemTrayIcon(parent)
  , _keyboard(keyboard)
  , _settings(settings)
  , _menu(new QMenu)
  , _actionGroup(new QActionGroup(this))
{
  // Connect signals-slots with keyboard object
  connect(this, SIGNAL(trigger()), _keyboard, SLOT(nextGroup()));

//  connect(_keyboard, SIGNAL(groupChanged(QXNLanguage::Language)), SLOT(keyboardGroupChanged(QXNLanguage::Language)));
  connect(_keyboard, SIGNAL(layoutChanged(uint)), SLOT(layoutChanged()));

  // Load the current state
  layoutChanged();

  // Internal connect to emit the correct signals
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  // Set icon menu
  _actionGroup->setExclusive(true);
  setContextMenu(_menu);
}


QXNTrayIcon::~QXNTrayIcon()
{}


void QXNTrayIcon::keyboardGroupChanged(QXNLanguage::Language language)
{
  // Show the tray icon
  setIcon(_iconMap[language]);

  // Show the tooltip
  setToolTip(tr("Current layout is %1").arg(QXNLanguage::languageToLocalizedText(language).toLower()));

  // Select the right action in context menu
  if (_languageActions.contains(language))
  {
    _languageActions.value(language)->setChecked(true);
  }
  else
  {
    qWarning() << "Unknown language detected in keyboard group change:" << language;
    qWarning() << "Available groups:" << _keyboard->groups();
  }
}


void QXNTrayIcon::layoutChanged()
{
  _keyboard->readKbdInfo();
  QXNLanguage::Language currentLanguage = _keyboard->currentLayout();

  // Do a cleanup
  _iconMap.clear();
  _menu->clear();
  _languageActions.clear();

  auto groups = _keyboard->groups();
  // Cache icons for currently used layouts and fill language menu
  for (int i = 0; i < groups.size(); ++i)
  {
    QXNLanguage::Language language = groups.at(i);

    QIcon icon = QXNLanguage::languageIcon(language);
    _iconMap[language] = icon;

    QAction* action =_menu->addAction(icon, QXNLanguage::languageToLocalizedText(language), this, SLOT(menuActionActivated()));
    action->setCheckable(true);
    action->setActionGroup(_actionGroup);
    action->setData(i);
    _languageActions[language] = action;
  }

  // Add static menu items
  if (_settings->value(QLatin1String("ShowQuitMenu"), true).toBool())
  {
    _menu->addSeparator();
    _menu->addAction(QIcon::fromTheme(QLatin1String("application-exit")), tr("Quit"), qApp, SLOT(quit()));
  }

  keyboardGroupChanged(currentLanguage);
}


void QXNTrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Trigger)
    emit trigger();
  else if (reason == QSystemTrayIcon::DoubleClick)
    emit doubleClicked();
}


void QXNTrayIcon::menuActionActivated()
{
  QAction* action = qobject_cast<QAction*>(sender());
  if (action)
    _keyboard->lockGroup(action->data().toUInt());
}
