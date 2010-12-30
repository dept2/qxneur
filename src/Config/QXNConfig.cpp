// Local
#include "QXNConfig.h"

// libxnconfig
extern "C"
{
#include <xneur/xnconfig.h>
#include <xneur/list_char.h>
}

// Qt
#include <QStringList>


// Static functions for conversion between QStringList and _list_char
QStringList listCharToStringList(_list_char* listChar)
{
  QStringList list;
  for (int i = 0; i < listChar->data_count; ++i)
    list.append(QString::fromLocal8Bit(listChar->data[i].string));

  return list;
}


struct _list_char* stringListToListChar(const QStringList& list)
{
  // Note that memory for the _list_char is allocated and must be freed manually
  _list_char* ret = list_char_init();

  for (int i = 0; i < list.size(); ++i)
    ret->add(ret, list[i].toLocal8Bit());

  return ret;
}


QXNConfig::QXNConfig(QObject* parent)
  : QObject(parent)
{
  // Initialize an object
  xnconfig = xneur_config_init();
  if (!xnconfig)
    qFatal("%s", qPrintable(tr("Error initializing xnconfig.")));

  // and load data
  load();

  // FIXME: temporary console logging
  /*
  for (int i=0; i<xnconfig->total_languages; i++)
  {
    qDebug("%s", qPrintable(QString("Group: %1, Name: %2")
                            .arg(xnconfig->get_lang_group(xnconfig, i))
                            .arg(xnconfig->get_lang_name(xnconfig, i))));
  }
  qDebug("%s", qPrintable(QString("Default group: %1 (%2 total)")
                          .arg(xnconfig->default_group)
                          .arg(xnconfig->total_languages)));
  */
}

QXNConfig::~QXNConfig()
{
  if (xnconfig)
  {
    xnconfig->uninit(xnconfig);
    xnconfig = 0;
  }
}


void QXNConfig::load()
{
  if (!xnconfig->load(xnconfig))
    qFatal("%s", qPrintable(tr("Error loading config")));
}

void QXNConfig::save()
{
  xnconfig->save(xnconfig);
}


// manual_mode parameter
bool QXNConfig::manualMode() const
{
  return xnconfig->manual_mode;
}

void QXNConfig::setManualMode(bool m)
{
  xnconfig->manual_mode = m;
}

void QXNConfig::setManualModeNow(bool m)
{
  if (manualMode() != m)
  {
    setManualMode(m);
    save();
  }
}


// educate parameter
bool QXNConfig::autoEducate() const
{
  return xnconfig->educate;
}

void QXNConfig::setAutoEducate(bool on)
{
  xnconfig->educate = on;
}


// dont_process_when_press_enter parameter
bool QXNConfig::noProcessOnEnter() const
{
  return xnconfig->dont_process_when_press_enter;
}

void QXNConfig::setNoProcessOnEnter(bool on)
{
  xnconfig->dont_process_when_press_enter = on;
}


// check_lang_on_process parameter
bool QXNConfig::checkLanguageDuringInput() const
{
  return xnconfig->check_lang_on_process;
}

void QXNConfig::setCheckLanguageDuringInput(bool on)
{
  xnconfig->check_lang_on_process = on;
}


// correct_incidental_caps parameter
bool QXNConfig::correctAccidentalCaps() const
{
  return xnconfig->correct_incidental_caps;
}

void QXNConfig::setCorrectAccidentalCaps(bool on)
{
  xnconfig->correct_incidental_caps = on;
}


// correct_two_capital_letter parameter
bool QXNConfig::correctTwoCapitalLetters() const
{
  return xnconfig->correct_two_capital_letter;
}

void QXNConfig::setCorrectTwoCapitalLetters(bool on)
{
  xnconfig->correct_two_capital_letter = on;
}


// correct_space_with_punctuation parameter
bool QXNConfig::correctSpaceWithPunctuation() const
{
  return xnconfig->correct_space_with_punctuation;
}

void QXNConfig::setCorrectSpaceWithPunctuation(bool on)
{
  xnconfig->correct_space_with_punctuation = on;
}


// disable_capslock parameter
bool QXNConfig::disableCaps() const
{
    return xnconfig->disable_capslock;
}

void QXNConfig::setDisableCaps(bool on)
{
    xnconfig->disable_capslock = on;
}


// save_selection parameter
bool QXNConfig::saveSelection() const
{
//  return xnconfig->save_selection;
  return true;
}

void QXNConfig::setSaveSelection(bool on)
{
//  xnconfig->save_selection = on;
}


bool QXNConfig::saveKeyboardLog() const
{
  return xnconfig->save_keyboard_log;
}

void QXNConfig::setSaveKeyboardLog(bool on)
{
  xnconfig->save_keyboard_log = on;
}


// flush_buffer_when_press_enter parameter
bool QXNConfig::flushInternalBuffers() const
{
  return xnconfig->flush_buffer_when_press_enter;
}

void QXNConfig::setFlushInternalBuffers(bool on)
{
  xnconfig->flush_buffer_when_press_enter = on;
}


// send_delay parameter
int QXNConfig::eventSendDelay() const
{
  return xnconfig->send_delay;
}

void QXNConfig::setEventSendDelay(int delay)
{
  xnconfig->send_delay = delay;
}


int QXNConfig::logLevel() const
{
  return xnconfig->log_level;
}

void QXNConfig::setLogLevel(int level)
{
  xnconfig->log_level = level;
}


// play_sounds parameter
bool QXNConfig::soundMode() const
{
  return xnconfig->play_sounds;
}

void QXNConfig::setSoundMode(bool m)
{
   xnconfig->play_sounds = m;
}

// sounds list
QString QXNConfig::actionSound(int action) const
{
  return QString::fromLocal8Bit(xnconfig->sounds[action].file);
}

void QXNConfig::setActionSound(int action, const QString& fileName)
{
  delete xnconfig->sounds[action].file;
  xnconfig->sounds[action].file = 0;
  char* str = fileName.toLocal8Bit().data();
  xnconfig->sounds[action].file = new char[strlen(str)+1];
  strcpy(xnconfig->sounds[action].file, str);
}

// volume_percent parameter
int QXNConfig::volumePercent() const
{
    return xnconfig->volume_percent;
}

void QXNConfig::setVolumePercent(int percent)
{
    xnconfig->volume_percent = percent;
}


QStringList QXNConfig::excludedApps() const
{
  return listCharToStringList(xnconfig->excluded_apps);
}


QStringList QXNConfig::layoutRememberApps() const
{
  return listCharToStringList(xnconfig->layout_remember_apps);
}


QStringList QXNConfig::autoApps() const
{
  return listCharToStringList(xnconfig->auto_apps);
}


QStringList QXNConfig::manualApps() const
{
  return listCharToStringList(xnconfig->manual_apps);
}


void QXNConfig::setExcludedApps(const QStringList& list)
{
  xnconfig->excluded_apps->uninit(xnconfig->excluded_apps);
  xnconfig->excluded_apps = stringListToListChar(list);
}


void QXNConfig::setLayoutRememberApps(const QStringList& list)
{
  xnconfig->layout_remember_apps->uninit(xnconfig->layout_remember_apps);
  xnconfig->layout_remember_apps = stringListToListChar(list);
}


void QXNConfig::setAutoApps(const QStringList& list)
{
  xnconfig->auto_apps->uninit(xnconfig->auto_apps);
  xnconfig->auto_apps = stringListToListChar(list);
}


void QXNConfig::setManualApps(const QStringList& list)
{
  xnconfig->manual_apps->uninit(xnconfig->manual_apps);
  xnconfig->manual_apps = stringListToListChar(list);
}


// remember_layout parameter
bool QXNConfig::rememberLayout() const
{
    return xnconfig->remember_layout;
}


void QXNConfig::setRememberLayout(bool on)
{
    xnconfig->remember_layout = on;
}


bool QXNConfig::ignoreLayoutForAbbreviations() const
{
  return xnconfig->abbr_ignore_layout;
}


void QXNConfig::setIgnoreLayoutForAbbreviations(bool on)
{
  xnconfig->abbr_ignore_layout = on;
}


StringToStringMap QXNConfig::abbreviations() const
{
  StringToStringMap ret;

  QStringList abbrList = listCharToStringList(xnconfig->abbreviations);
  for (int i=0; i<abbrList.size(); i++)
  {
    // Split the abbreviation string
    QString abbrString = abbrList[i];
    int splitPosition = abbrString.indexOf(QChar::fromAscii(' '));
    QString abbr = abbrString.left(splitPosition);
    QString full = abbrString.mid(splitPosition+1);

    ret[abbr] = full;
  }

  return ret;
}


void QXNConfig::setAbbreviations(const StringToStringMap& list)
{
  QStringList abbrList;

  QMapIterator<QString,QString> it(list);
  while (it.hasNext())
  {
    it.next();
    abbrList.append(QString(QLatin1String("%1 %2")).arg(it.key()).arg(it.value()));
  }

  xnconfig->abbreviations->uninit(xnconfig->abbreviations);
  xnconfig->abbreviations = stringListToListChar(abbrList);
}


QString QXNConfig::version() const
{
  return QString::fromLocal8Bit(xnconfig->version);
}


// show_osd parameter
bool QXNConfig::showOSD() const
{
    return xnconfig->show_osd;
}

void QXNConfig::setShowOSD(bool on)
{
    xnconfig->show_osd = on;
}


// show_popups parameter
bool QXNConfig::showPopups() const
{
    return xnconfig->show_popup;
}


void QXNConfig::setShowPopups(bool on)
{
    xnconfig->show_popup = on;
}
