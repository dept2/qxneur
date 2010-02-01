// C++ Interface: QXNKeyboardPrivate
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009
#ifndef QXNKEYBOARD_P_H
#define QXNKEYBOARD_P_H

// Qt
#include <QList>

// Local
#include "QXNLanguage.h"
class QXNKeyboard;


class QXNKeyboardPrivate
{
  Q_DECLARE_PUBLIC(QXNKeyboard);

  private:
    QXNKeyboardPrivate();
    ~QXNKeyboardPrivate();

    void loadCurrentGroup();
    void loadGroupCount();
    void loadGroupNames();

    void reload();

    int _eventCode;

    int _currentGroup;
    int _groupCount;
    QList<QXNLanguage::Language> _groups;

    QXNKeyboard* q_ptr;
};

#endif // QXNKEYBOARD_P_H
