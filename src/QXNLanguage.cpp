// C++ Implementation: QXNLanguage
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009

// Local
#include "QXNLanguage.h"

// Qt
#include <QMetaEnum>
#include <QDebug>


QXNLanguage::QXNLanguage(QObject* parent)
  : QObject(parent)
{}


QXNLanguage::~QXNLanguage()
{}


QXNLanguage::Language QXNLanguage::layoutNameToLanguage(const QString& name)
{
  if (name == QLatin1String("Belarusian"))
    return Belarusian;
  else if (name == QLatin1String("Bulgarian"))
    return Bulgarian;
  else if (name == QLatin1String("Czech"))
    return Czech;
  else if (name == QLatin1String("Germany") || name == QLatin1String("Swiss German"))
    return German;
  else if (name == QLatin1String("Danish"))
    return Danish;
  else if (name == QLatin1String("Canadian") || name == QLatin1String("Great Britain")
           || name == QLatin1String("US/ASCII") || name == QLatin1String("USA")
           || name == QLatin1String("English") || name == QLatin1String("U.S. English")
           || name == QLatin1String("English (US)"))
    return English;
  else if (name == QLatin1String("Spanish"))
    return Spanish;
  else if (name == QLatin1String("Finnish"))
    return Finnish;
  else if (name == QLatin1String("French") || name == QLatin1String("Swiss French"))
    return French;
  else if (name == QLatin1String("Georgian"))
    return Georgian;
  else if (name == QLatin1String("Greek") || name == QLatin1String("ISO8859-7"))
    return Greek;
  else if (name == QLatin1String("Hungarian"))
    return Hungarian;
  else if (name == QLatin1String("Kazakh"))
    return Kazakh;
  else if (name == QLatin1String("Icelandic"))
    return Icelandic;
  else if (name == QLatin1String("Italian"))
    return Italian;
  else if (name == QLatin1String("Lithuanian"))
    return Lithuanian;
  else if (name == QLatin1String("Latvian"))
    return Latvian;
  else if (name == QLatin1String("Macedonian"))
    return Macedonian;
  else if (name == QLatin1String("Norwegian"))
    return Norwegian;
  else if (name == QLatin1String("Polish"))
    return Polish;
  else if (name == QLatin1String("Portugese"))
    return Portugese;
  else if (name == QLatin1String("Romanian"))
    return Romanian;
  else if (name == QLatin1String("Russia") || name == QLatin1String("Russia - Winkeys")
           || name == QLatin1String("Russian"))
    return Russian;
  else if (name == QLatin1String("Slovak"))
    return Slovak;
  else if (name == QLatin1String("Swedish"))
    return Swedish;
  else if (name == QLatin1String("Thai"))
    return Thai;
  else if (name == QLatin1String("Ukraine") || name == QLatin1String("Ukraine - Winkeys")
           || name == QLatin1String("Ukrainian"))
    return Ukrainian;
  else if (name == QLatin1String("Vietnamese"))
    return Vietnamese;
  else
  {
    qWarning() << "Unsupported language:" << name;
    return Unknown;
  }
}


QString QXNLanguage::languageToText(Language language)
{
  // Kind of hack to show the language name and not embed a string with it to code directly
  QMetaEnum lang = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Language"));
  return QLatin1String(lang.valueToKey(language));
}


QString QXNLanguage::languageToLocalizedText(Language language)
{
  if (language==Armenian)
    return tr("Armenian");
  else if (language==Belarusian)
    return tr("Belarusian");
  else if (language==Bulgarian)
    return tr("Bulgarian");
  else if (language==Czech)
    return tr("Czech");
  else if (language==English)
    return tr("English");
  else if (language==Estonian)
    return tr("Estonian");
  else if (language==French)
    return tr("French");
  else if (language==German)
    return tr("German");
  else if (language==Kazakh)
    return tr("Kazakh");
  else if (language==Latvian)
    return tr("Latvian");
  else if (language==Lithuanian)
    return tr("Lithuanian");
  else if (language==Polish)
    return tr("Polish");
  else if (language==Romanian)
    return tr("Romanian");
  else if (language==Russian)
    return tr("Russian");
  else if (language==Spanish)
    return tr("Spanish");
  else if (language==Ukrainian)
    return tr("Ukrainian");
  else
    return tr("Unknown");
}


QIcon QXNLanguage::languageIcon(Language language)
{
  return QIcon( QString(QLatin1String(":/flags/%1.png")).arg(QXNLanguage::languageToText(language)) );
}
