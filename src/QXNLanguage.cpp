// C++ Implementation: QXNLanguage
// Author: Boris Moiseev <cyberbobs@gmail.com>, (C) 2009

// Local
#include "QXNLanguage.h"

// Qt
#include <QMetaEnum>

QXNLanguage::QXNLanguage(QObject* parent)
  : QObject(parent)
{
}

QXNLanguage::~QXNLanguage()
{
}


QXNLanguage::Language QXNLanguage::layoutNameToLanguage(const QString& name)
{
  if (name=="Belarusian")
    return Belarusian;
  else if (name=="Bulgarian")
    return Bulgarian;
  else if (name=="Czech")
    return Czech;
  else if (name=="Germany" || name=="Swiss German")
    return German;
  else if (name=="Danish")
    return Danish;
  else if (name=="Canadian" || name=="Great Britain" || name=="US/ASCII" || name=="USA" || name=="English"
           || name=="U.S. English")
    return English;
  else if (name=="Spanish")
    return Spanish;
  else if (name=="Finnish")
    return Finnish;
  else if (name=="French" || name=="Swiss French")
    return French;
  else if (name=="Georgian")
    return Georgian;
  else if (name=="Greek" || name=="ISO8859-7")
    return Greek;
  else if (name=="Hungarian")
    return Hungarian;
  else if (name=="Kazakh")
    return Kazakh;
  else if (name=="Icelandic")
    return Icelandic;
  else if (name=="Italian")
    return Italian;
  else if (name=="Lithuanian")
    return Lithuanian;
  else if (name=="Latvian")
    return Latvian;
  else if (name=="Macedonian")
    return Macedonian;
  else if (name=="Norwegian")
    return Norwegian;
  else if (name=="Polish")
    return Polish;
  else if (name=="Portugese")
    return Portugese;
  else if (name=="Romanian")
    return Romanian;
  else if (name=="Russia" || name=="Russia - Winkeys" || name=="Russian")
    return Russian;
  else if (name=="Slovak")
    return Slovak;
  else if (name=="Swedish")
    return Swedish;
  else if (name=="Thai")
    return Thai;
  else if (name=="Ukraine" || name=="Ukraine - Winkeys" || name=="Ukrainian")
    return Ukrainian;
  else if (name=="Vietnamese")
    return Vietnamese;
  else
    return Unknown;
}


QString QXNLanguage::languageToText(Language language)
{
  // Kind of hack to show the language name and not embed a string with it to code directly
  QMetaEnum lang = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Language"));
  return lang.valueToKey(language);
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
  return QIcon( QString(":/flags/%1.png").arg(QXNLanguage::languageToText(language)) );
}

