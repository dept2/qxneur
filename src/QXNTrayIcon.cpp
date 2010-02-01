// C++ Implementation: QXNTrayIcon
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009

// Local
#include "QXNTrayIcon.h"
#include "QXNKeyboard.h"


QXNTrayIcon::QXNTrayIcon(QXNKeyboard* keyboard, QObject* parent)
  : QSystemTrayIcon(parent), _keyboard(keyboard)
{
  // Connect signals-slots with keyboard object
  connect(this, SIGNAL(trigger()), _keyboard, SLOT(nextGroup()));

  connect(_keyboard, SIGNAL(groupChanged(QXNLanguage::Language)),
          SLOT(keyboardGroupChanged(QXNLanguage::Language)));
  connect(_keyboard, SIGNAL(layoutChanged()),
          SLOT(layoutChanged()));

  // Load the current state
  layoutChanged();

  // Internal connect to emit the correct signals
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

QXNTrayIcon::~QXNTrayIcon()
{
}


void QXNTrayIcon::keyboardGroupChanged(QXNLanguage::Language language)
{
  // Show the tray icon
  setIcon(_iconMap[language]);

  // Show the tooltip
  setToolTip(tr("QXNeur: current layout is %1").arg(QXNLanguage::languageToLocalizedText(language).toLower()));
}

void QXNTrayIcon::layoutChanged()
{
  // Cache icons for currently used layouts
  _iconMap.clear();
  QList<QXNLanguage::Language> groupList = _keyboard->groups();
  foreach (QXNLanguage::Language language, groupList)
    _iconMap[language] = QXNLanguage::languageIcon(language);

  // And reload current icon
  keyboardGroupChanged(_keyboard->currentGroup());
}


void QXNTrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Trigger)
    emit trigger();
  else if (reason == QSystemTrayIcon::DoubleClick)
    emit doubleClicked();
}
