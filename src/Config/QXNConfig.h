#ifndef QXNCONFIG_H
#define QXNCONFIG_H

// Qt
#include <QObject>
#include <QMap>

// libxnconfig
class _xneur_config;
class _list_char;

typedef QMap<QString,QString> StringToStringMap;

class QXNConfig : public QObject
{
  Q_OBJECT
  public:
    QXNConfig(QObject* parent=0);
    ~QXNConfig();

    // Working mode options
    bool manualMode() const;
    bool autoEducate() const;
    bool noProcessOnEnter() const;
    bool checkLanguageDuringInput() const;
    bool saveKeyboardLog() const;

    // Correction options
    bool correctAccidentalCaps() const;
    bool correctTwoCapitalLetters() const;
    bool correctSpaceWithPunctuation() const;

    // Advanced options
    bool saveSelection() const;
    bool flushInternalBuffers() const;
    int eventSendDelay() const;
    int logLevel() const;

    // Sounds
    bool soundMode() const;
    QString actionSound(int action) const;

    // Applications
    QStringList excludedApps() const;
    QStringList layoutRememberApps() const;
    QStringList autoApps() const;
    QStringList manualApps() const;

    // Abbreviations
    bool ignoreLayoutForAbbreviations() const;
    StringToStringMap abbreviations() const;

  public slots:
    void load();
    void save();

    void setManualModeNow(bool);

    // Working mode options
    void setManualMode(bool);
    void setAutoEducate(bool);
    void setNoProcessOnEnter(bool);
    void setCheckLanguageDuringInput(bool);
    void setSaveKeyboardLog(bool);

    // Correction options
    void setCorrectAccidentalCaps(bool);
    void setCorrectTwoCapitalLetters(bool);
    void setCorrectSpaceWithPunctuation(bool);

    // Advanced options
    void setSaveSelection(bool);
    void setFlushInternalBuffers(bool);
    void setEventSendDelay(int);
    void setLogLevel(int);

    // Sounds
    void setSoundMode(bool on);
    void setActionSound(int action, const QString& fileName);

    // Applications
    void setExcludedApps(const QStringList&);
    void setLayoutRememberApps(const QStringList&);
    void setAutoApps(const QStringList&);
    void setManualApps(const QStringList&);

    // Abbreviations
    void setIgnoreLayoutForAbbreviations(bool);
    void setAbbreviations(const StringToStringMap&);

  protected:
    _xneur_config* xnconfig;

    static QStringList listCharToStringList(_list_char*);
    struct _list_char* stringListToListChar(const QStringList&);
};

#endif // QXNCONFIG_H
