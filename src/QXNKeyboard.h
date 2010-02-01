// C++ Interface: QXNKeyboard
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009
#ifndef QXNKEYBOARD_H
#define QXNKEYBOARD_H

// Qt
#include <QApplication>
#include <QObject>

// Local
#include "QXNLanguage.h"
class QXNKeyboardPrivate;


class QXNKeyboard : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QXNKeyboard);

  public:
    QXNKeyboard(QObject* parent=0);
    ~QXNKeyboard();

    int groupCount();
    QList<QXNLanguage::Language> groups();
    QXNLanguage::Language currentGroup();

    void x11Event(XEvent* event);

  signals:
    void groupChanged(QXNLanguage::Language);
    void layoutChanged();

  public slots:
    void nextGroup();

  protected:
    QXNKeyboardPrivate* d_ptr;
};

#endif // QXNKEYBOARD_H
