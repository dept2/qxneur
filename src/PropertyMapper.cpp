// Local
#include "PropertyMapper.h"
#include "PropertyMapperPrivate.h"

// Qt
#include <QMetaProperty>


PropertyMapper::PropertyMapper(QObject* parent)
  : QObject(parent),
    d_ptr(new PropertyMapperPrivate(this))
{}


PropertyMapper::~PropertyMapper()
{}


bool PropertyMapper::setMapping(QObject* source, const char* sourcePropertyName, const char* changeSignal,
                                QObject* target, const char* targetPropertyName, MappingMode mode)
{
  Q_D(PropertyMapper);

  if (!source || !sourcePropertyName || !target || !targetPropertyName)
  {
    qWarning("<PropertyMapper::setMapping> One of the needed parameters is null");
    return false;
  }

  // Getting source property and checking it
  const QMetaObject* sourceMeta = source->metaObject();
  QMetaProperty sourceProperty = sourceMeta->property(sourceMeta->indexOfProperty(sourcePropertyName));
  if (!(sourceProperty.isValid() && sourceProperty.isReadable() && sourceProperty.isWritable()))
  {
    qWarning("<PropertyMapper::setMapping> Property with name \"%s\" does not exist or can not be accessed",
             sourcePropertyName);
    return false;
  }

  // Getting target property and checking it
  const QMetaObject* targetMeta = target->metaObject();
  QMetaProperty targetProperty = targetMeta->property(targetMeta->indexOfProperty(targetPropertyName));
  if (!(targetProperty.isValid() && targetProperty.isReadable() && targetProperty.isWritable()))
  {
    qWarning("<PropertyMapper::setMapping> Property with name \"%s\" does not exist or can not be accessed",
             targetPropertyName);
    return false;
  }

  // Checking if the property have the notify signal
  if (!changeSignal && !sourceProperty.hasNotifySignal())
  {
    qWarning("<PropertyMapper::setMapping> Property with name \"%s\" have no notify signal and no custom notify signal"
             " specified", sourcePropertyName);
    return false;
  }

  // Check if the source property may be converted to target property type
  if (sourceProperty.type() != targetProperty.type() && !sourceProperty.read(source).canConvert(targetProperty.type()))
  {
    qWarning("<PropertyMapper::setMapping> Source property can not be converted to target property type");
    return false;
  }

  bool result;
  if (changeSignal)
    result = connect(source, changeSignal, SLOT(_q_propertyChanged()));
  else
  {
    // Use some black magic to generate a signal name
    QByteArray s = QMetaObject::normalizedSignature(sourceProperty.notifySignal().signature());
    s.prepend(QSIGNAL_CODE + '0');
    result = connect(source, s, SLOT(_q_propertyChanged()));
  }

  // Store the mapping if the connection went okay
  if (result)
    d->propertyMapping.insertMulti(source, Mapping(sourcePropertyName, target, targetPropertyName, mode));

  return result;
}


bool PropertyMapper::setMapping(QObject* source, const char* sourcePropertyName,
                                QObject* target, const char* targetPropertyName, MappingMode mode)
{
  return setMapping(source, sourcePropertyName, 0, target, targetPropertyName, mode);
}



bool PropertyMapper::setMapping(QObject* source, QObject* target, const char* targetPropertyName, MappingMode mode)
{
  QMetaProperty property = source->metaObject()->userProperty();

  if (property.isValid())
    return setMapping(source, property.name(), target, targetPropertyName, mode);
  else
  {
    qWarning("<PropertyMapper::setMapping> Source QObject doesn't have a USER property. "
             "Default mapping can not be added");
    return false;
  }
}


void PropertyMapper::removeMappings(QObject* source)
{
  Q_D(PropertyMapper);

  d->propertyMapping.remove(source);
}


bool PropertyMapper::isChanged()
{
  Q_D(PropertyMapper);

  bool ret = false;
  for (ObjectMappingMap::iterator it=d->propertyMapping.begin(); it!=d->propertyMapping.end(); ++it)
  {
    if ((*it).changedFlag)
    {
      ret = true;
      break;
    }
  }

  return ret;
}


void PropertyMapper::load()
{
  Q_D(PropertyMapper);

  // Set the flag for the d object to ignore signals from attached objects
  d->ignoreChanges = true;

  // Cache the parent meta object pointer

  // Cycle through the mappings
  bool loaded = true;
  for (ObjectMappingMap::iterator it=d->propertyMapping.begin(); it!=d->propertyMapping.end(); ++it)
  {
    // Get the target property value
    const QMetaObject* targetMeta = (*it).targetObject->metaObject();
    QVariant target = targetMeta->property(targetMeta->indexOfProperty((*it).targetProperty)).read((*it).targetObject);

    if (target.isValid())
    {
      // Try to set the source property
      const QMetaObject* sourceMeta = it.key()->metaObject();
      const QMetaProperty& sourceProperty = sourceMeta->property(sourceMeta->indexOfProperty((*it).sourceProperty));
      bool ok = PropertyMapperPrivate::writeProperty(it.key(), sourceProperty, target);

      if (ok)
        (*it).changedFlag = false;
      else
        loaded = false;
    }
    else
      qWarning("<PropertyMapper::load> Can not read the value of property \"%s\" from target object",
               (*it).targetProperty);
  }

  // Unset the flag and emit signal
  d->ignoreChanges = false;
  emit propertiesChanged(!loaded);
}


void PropertyMapper::apply()
{
  Q_D(PropertyMapper);

  // Cycle through the mappings
  bool applied = true;
  for (ObjectMappingMap::iterator it=d->propertyMapping.begin(); it!=d->propertyMapping.end(); ++it)
  {
    const QMetaObject* sourceMeta = it.key()->metaObject();
    QVariant source = sourceMeta->property(sourceMeta->indexOfProperty((*it).sourceProperty)).read(it.key());

    if (source.isValid())
    {
      const QMetaObject* targetMeta = (*it).targetObject->metaObject();
      const QMetaProperty& targetProperty = targetMeta->property(targetMeta->indexOfProperty((*it).targetProperty));
      bool ok = PropertyMapperPrivate::writeProperty((*it).targetObject, targetProperty, source);

      if (ok)
        (*it).changedFlag = false;
      else
        applied = false;
    }
    else
      qWarning("<PropertyMapper::apply> Can not read the value of property \"%s\" from target object",
               (*it).targetProperty);
  }

  emit propertiesChanged(!applied);
}


PropertyMapperPrivate::PropertyMapperPrivate(PropertyMapper* parent)
  : q_ptr(parent), ignoreChanges(false)
{}


void PropertyMapperPrivate::_q_propertyChanged()
{
  Q_Q(PropertyMapper);

  // If the ignoring flag is not set
  if (!ignoreChanges)
  {
    QObject* sender = q->sender();
    Q_ASSERT(sender != 0);

    // Cache the meta object pointers
    const QMetaObject* sourceMeta = sender->metaObject();

    // And store the variable containing the summary changed flag
    bool changedFlag = false;

    // Cycle through the mappings
    for (ObjectMappingMap::iterator it=propertyMapping.begin(); it!=propertyMapping.end(); ++it)
    {
      if (it.key() == sender)
      {
        const QMetaObject* targetMeta = (*it).targetObject->metaObject();

        QVariant source = sourceMeta->property(sourceMeta->indexOfProperty((*it).sourceProperty)).read(sender);

        QMetaProperty targetProperty = targetMeta->property(targetMeta->indexOfProperty((*it).targetProperty));
        QVariant target = targetProperty.read((*it).targetObject);

        if ((*it).mode == PropertyMapper::StoreChanged)
          (*it).changedFlag = (source != target);
        else
          writeProperty((*it).targetObject, targetProperty, source);
      }

      if ((*it).changedFlag)
        changedFlag = true;
    }

    // And emit signal
    if (!q->metaObject()->invokeMethod(q, "propertiesChanged", Qt::DirectConnection, Q_ARG(bool,changedFlag)))
      qWarning("<PropertyMapper::_q_propertyChanged> Could not invoke the \"propertiesChanged\" signal");
  }
}


bool PropertyMapperPrivate::writeProperty(QObject* object, const QMetaProperty& property, const QVariant& value)
{
  bool ok = false;

  QVariant::Type propertyType = property.type();
  if (propertyType == value.type())
    ok = property.write(object, value);
  else if (value.canConvert(propertyType))
  {
    QVariant v = value;

    ok = v.convert(propertyType);
    if (ok)
      ok = property.write(object, v);
    else
      qWarning("<PropertyMapperPrivate::writeProperty> Data can not be converted to the type of property");
  }

  if (!ok)
    qWarning("<PropertyMapperPrivate::writeProperty> Can not set the value of property \"%s\"", property.name());

  return ok;
}
