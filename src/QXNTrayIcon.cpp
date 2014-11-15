// Local
#include "QXNTrayIcon.h"
#include "QXNKeyboard.h"

// Qt
#include <QMenu>
#include <QSettings>


QXNTrayIcon::QXNTrayIcon(QXNKeyboard* keyboard, QSettings* settings, QObject* parent)
  : QSystemTrayIcon(parent)
  , _keyboard(keyboard)
  , _settings(settings)
  , _menu(new QMenu)
  , _actionGroup(new QActionGroup(this))
{
  // Connect signals-slots with keyboard object
  connect(this, SIGNAL(trigger()), _keyboard, SLOT(nextGroup()));

  connect(_keyboard, SIGNAL(groupChanged(QXNLanguage::Language)), SLOT(keyboardGroupChanged(QXNLanguage::Language)));
  connect(_keyboard, SIGNAL(layoutChanged()), SLOT(layoutChanged()));

  // Load the current state
  layoutChanged();

  // Internal connect to emit the correct signals
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

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
  _languageActions.value(language)->setChecked(true);
}


void QXNTrayIcon::layoutChanged()
{
  QList<QXNLanguage::Language> groupList = _keyboard->groups();
  QXNLanguage::Language currentGroup = _keyboard->currentGroup();

  // Do a cleanup
  _iconMap.clear();
  _menu->clear();
  _languageActions.clear();

  // Cache icons for currently used layouts and fill language menu
  foreach (QXNLanguage::Language language, groupList)
  {
    QIcon icon = QXNLanguage::languageIcon(language);
    _iconMap[language] = icon;

    QAction* action =_menu->addAction(icon, QXNLanguage::languageToLocalizedText(language),
                                      this, SLOT(menuActionActivated()));
    action->setCheckable(true);
    action->setActionGroup(_actionGroup);
    action->setData(int(language));
    _languageActions[language] = action;
  }

  // Add static menu items
  if (_settings->value(QLatin1String("ShowQuitMenu"), true).toBool())
  {
    _menu->addSeparator();
    _menu->addAction(QIcon::fromTheme(QLatin1String("application-exit")), tr("Quit"),
                     qApp, SLOT(quit()));
  }

  // And reload current icon
  keyboardGroupChanged(currentGroup);
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
  {
    QXNLanguage::Language language = QXNLanguage::Language(action->data().toInt());
    emit keyboardGroupRequested(language);
  }
}
