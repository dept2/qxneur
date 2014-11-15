// C++ Interface: QXNTrayIcon
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009
#ifndef QXNTRAYICON_H
#define QXNTRAYICON_H

// Qt
#include <QSystemTrayIcon>
#include <QMap>
class QMenu;
class QAction;
class QActionGroup;
class QSettings;

// Local
#include "QXNLanguage.h"
class QXNKeyboard;


class QXNTrayIcon : public QSystemTrayIcon
{
  Q_OBJECT

  public:
    QXNTrayIcon(QXNKeyboard* keyboard, QSettings* settings, QObject* parent=0);
    ~QXNTrayIcon();

  signals:
    void trigger();
    void doubleClicked();
    void keyboardGroupRequested(QXNLanguage::Language);

  public slots:
    void keyboardGroupChanged(QXNLanguage::Language);
    void layoutChanged();

  protected slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void menuActionActivated();

  protected:
    QMap<QXNLanguage::Language,QIcon> _iconMap;
    QMap<QXNLanguage::Language,QAction*> _languageActions;

    QXNKeyboard* _keyboard;
    QSettings* _settings;
    QMenu* _menu;
    QActionGroup* _actionGroup;
};

#endif // QXNTRAYICON_H
