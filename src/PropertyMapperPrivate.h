#ifndef PROPERTYMAPPERPRIVATE_H
#define PROPERTYMAPPERPRIVATE_H

// Local
#include "PropertyMapper.h"

// Qt
#include <QPair>
#include <QMultiMap>


struct Mapping
{
  Mapping(const char* sourceProperty, QObject* targetObject, const char* targetProperty,
          PropertyMapper::MappingMode mode)
    : sourceProperty(sourceProperty), targetObject(targetObject), targetProperty(targetProperty),
      mode(mode), changedFlag(false)
  {}

  const char* sourceProperty;
  QObject* targetObject;
  const char* targetProperty;
  PropertyMapper::MappingMode mode;
  bool changedFlag;
};

typedef QMultiMap<QObject*, Mapping> ObjectMappingMap;


class PropertyMapperPrivate
{
  Q_DECLARE_PUBLIC(PropertyMapper)

  private:
    PropertyMapper* q_ptr;
    PropertyMapperPrivate(PropertyMapper* parent);

    ObjectMappingMap propertyMapping;
    bool ignoreChanges;

    void _q_propertyChanged();
    static bool writeProperty(QObject* object, const QMetaProperty& property, const QVariant& value);
};

#endif // PROPERTYMAPPERPRIVATE_H
