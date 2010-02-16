#ifndef PROPERTYMAPPER_H
#define PROPERTYMAPPER_H

// Qt
#include <QObject>

class PropertyMapperPrivate;
class PropertyMapper : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isChanged READ isChanged NOTIFY propertiesChanged)

  public:
    PropertyMapper(QObject* parent=0);
    ~PropertyMapper();

    enum MappingMode { StoreChanged, ApplyOnChange };

    bool setMapping(QObject* source, const char* sourcePropertyName, const char* changeSignal,
                    QObject* target, const char* targetPropertyName, MappingMode mode = StoreChanged);
    bool setMapping(QObject* source, const char* sourcePropertyName,
                    QObject* target, const char* targetPropertyName, MappingMode mode = StoreChanged);
    bool setMapping(QObject* source, QObject* target, const char* targetPropertyName, MappingMode mode = StoreChanged);

    void removeMappings(QObject* source);

    bool isChanged();

  signals:
    void propertiesChanged(bool);

  public slots:
    void load();
    void apply();

  private:
    QScopedPointer<PropertyMapperPrivate> d_ptr;
    Q_DECLARE_PRIVATE(PropertyMapper)
    Q_PRIVATE_SLOT(d_func(), void _q_propertyChanged())
};

#endif // PROPERTYMAPPER_H
