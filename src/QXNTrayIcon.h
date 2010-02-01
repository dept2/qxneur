// C++ Interface: QXNTrayIcon
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009
#ifndef QXNTRAYICON_H
#define QXNTRAYICON_H

// Qt
#include <QSystemTrayIcon>
#include <QMap>

// Local
#include "QXNLanguage.h"
class QXNKeyboard;


class QXNTrayIcon : public QSystemTrayIcon
{
  Q_OBJECT

  public:
    QXNTrayIcon(QXNKeyboard* keyboard, QObject* parent=0);
    ~QXNTrayIcon();

  signals:
    void trigger();
    void doubleClicked();

  public slots:
    void keyboardGroupChanged(QXNLanguage::Language);
    void layoutChanged();

  protected slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);

  protected:
    QMap<QXNLanguage::Language,QIcon> _iconMap;
    QXNKeyboard* _keyboard;
};

#endif // QXNTRAYICON_H
