// C++ Interface: QXNLanguage
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009
#ifndef QXNLANGUAGE_H
#define QXNLANGUAGE_H

// Qt
#include <QObject>
#include <QIcon>


class QXNLanguage : public QObject
{
  Q_OBJECT
  Q_ENUMS(Language)

  public:
    QXNLanguage(QObject* parent=0);
    ~QXNLanguage();

    enum Language { Unknown, Armenian, Belarusian, Bulgarian, Czech, German, Danish, English, Estonian, Spanish,
      Finnish, French, Georgian, Greek, Hungarian, Kazakh, Israelian, Icelandic, Italian, Lithuanian, Latvian,
      Macedonian, Norwegian, Polish, Portugese, Romanian, Russian, Slovak, Swedish, Thai, Ukrainian, Vietnamese};

    static Language layoutNameToLanguage(const QString&);
    static QString languageToText(Language);
    static QString languageToLocalizedText(Language);
    static QIcon languageIcon(Language);
};

#endif // QXNLANGUAGE_H
