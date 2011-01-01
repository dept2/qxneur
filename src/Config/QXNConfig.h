#ifndef QXNCONFIG_H
#define QXNCONFIG_H

// Qt
#include <QObject>
#include <QMap>
#include <QStringList>

// libxnconfig
struct _xneur_config;
struct _list_char;

typedef QMap<QString,QString> StringToStringMap;

class QXNConfig : public QObject
{
  Q_OBJECT

  // Working mode options
  Q_PROPERTY(bool manualMode READ manualMode WRITE setManualMode)
  Q_PROPERTY(bool autoEducate READ autoEducate WRITE setAutoEducate)
  Q_PROPERTY(bool noProcessOnEnter READ noProcessOnEnter WRITE setNoProcessOnEnter)
  Q_PROPERTY(bool checkLanguageDuringInput READ checkLanguageDuringInput WRITE setCheckLanguageDuringInput)
  Q_PROPERTY(bool saveKeyboardLog READ saveKeyboardLog WRITE setSaveKeyboardLog)

  // Correction options
  Q_PROPERTY(bool correctAccidentalCaps READ correctAccidentalCaps WRITE setCorrectAccidentalCaps)
  Q_PROPERTY(bool correctTwoCapitalLetters READ correctTwoCapitalLetters WRITE setCorrectTwoCapitalLetters)
  Q_PROPERTY(bool correctSpaceWithPunctuation READ correctSpaceWithPunctuation WRITE setCorrectSpaceWithPunctuation)
  Q_PROPERTY(bool disableCaps READ disableCaps WRITE setDisableCaps)

  // Advanced options
  Q_PROPERTY(bool saveSelectionAfterConvert READ saveSelectionAfterConvert WRITE setSaveSelectionAfterConvert)
  Q_PROPERTY(bool flushInternalBuffers READ flushInternalBuffers WRITE setFlushInternalBuffers)
  Q_PROPERTY(int eventSendDelay READ eventSendDelay WRITE setEventSendDelay)
  Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel)

  // Sounds
  Q_PROPERTY(bool soundMode READ soundMode WRITE setSoundMode)
  Q_PROPERTY(int volumePercent READ volumePercent WRITE setVolumePercent)

  // Applications
  Q_PROPERTY(QStringList excludedApps READ excludedApps WRITE setExcludedApps)
  Q_PROPERTY(QStringList layoutRememberApps READ layoutRememberApps WRITE setLayoutRememberApps)
  Q_PROPERTY(QStringList autoApps READ autoApps WRITE setAutoApps)
  Q_PROPERTY(QStringList manualApps READ manualApps WRITE setManualApps)
  Q_PROPERTY(bool rememberLayout READ rememberLayout WRITE setRememberLayout)

  // Abbreviations
  Q_PROPERTY(bool ignoreLayoutForAbbreviations READ ignoreLayoutForAbbreviations WRITE setIgnoreLayoutForAbbreviations)
  Q_PROPERTY(StringToStringMap abbreviations READ abbreviations WRITE setAbbreviations)

  // Popups
  Q_PROPERTY(bool showOSD READ showOSD WRITE setShowOSD)
  Q_PROPERTY(bool showPopups READ showPopups WRITE setShowPopups)

  // Version
  Q_PROPERTY(QString version READ version)

  public:
    explicit QXNConfig(QObject* parent = 0);
    ~QXNConfig();

    // Working mode options
    bool manualMode() const;
    void setManualMode(bool);

    bool autoEducate() const;
    void setAutoEducate(bool);

    bool noProcessOnEnter() const;
    void setNoProcessOnEnter(bool);

    bool checkLanguageDuringInput() const;
    void setCheckLanguageDuringInput(bool);

    bool saveKeyboardLog() const;
    void setSaveKeyboardLog(bool);

    // Correction options
    bool correctAccidentalCaps() const;
    void setCorrectAccidentalCaps(bool);

    bool correctTwoCapitalLetters() const;
    void setCorrectTwoCapitalLetters(bool);

    bool correctSpaceWithPunctuation() const;
    void setCorrectSpaceWithPunctuation(bool);

    bool disableCaps() const;
    void setDisableCaps(bool);

    // Advanced options
    bool saveSelectionAfterConvert() const;
    void setSaveSelectionAfterConvert(bool);

    bool flushInternalBuffers() const;
    void setFlushInternalBuffers(bool);

    int eventSendDelay() const;
    void setEventSendDelay(int);

    int logLevel() const;
    void setLogLevel(int);

    // Sounds
    bool soundMode() const;
    void setSoundMode(bool on);

    QString actionSound(int action) const;
    void setActionSound(int action, const QString& fileName);

    int volumePercent() const;
    void setVolumePercent(int);

    // Applications
    QStringList excludedApps() const;
    void setExcludedApps(const QStringList&);

    QStringList layoutRememberApps() const;
    void setLayoutRememberApps(const QStringList&);

    QStringList autoApps() const;
    void setAutoApps(const QStringList&);

    QStringList manualApps() const;
    void setManualApps(const QStringList&);

    bool rememberLayout() const;
    void setRememberLayout(bool);

    // Abbreviations
    bool ignoreLayoutForAbbreviations() const;
    void setIgnoreLayoutForAbbreviations(bool);

    StringToStringMap abbreviations() const;
    void setAbbreviations(const StringToStringMap&);

    // Popups
    bool showOSD() const;
    void setShowOSD(bool);

    bool showPopups() const;
    void setShowPopups(bool);

    // Version
    QString version() const;

  signals:
    void configurationSaved();

  public slots:
    void load();
    void save();

    void setManualModeNow(bool);

  protected:
    _xneur_config* xnconfig;
};

#endif // QXNCONFIG_H
