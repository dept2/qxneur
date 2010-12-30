// Local
#include "QXNApplicationsModel.h"

// Qt
#include <QStringList>


QXNApplicationsModel::QXNApplicationsModel(QObject* parent)
  : QAbstractTableModel(parent)
{ }


int QXNApplicationsModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;

  return m_data.size();
}


int QXNApplicationsModel::columnCount(const QModelIndex& parent) const
{
  return 3;
}


QVariant QXNApplicationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    switch (section)
    {
    case 0: return tr("Application");
    case 1: return tr("Whole application layout");
    case 2: return tr("Switching");
    }
  }

  return QVariant();
}


QVariant QXNApplicationsModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  const QXNApplicationsModelPrivate& rowData = m_data.at(index.row());

  if (role == Qt::DisplayRole)
  {
    switch (index.row())
    {
    case 0: return rowData.appName;
    case 2:
    {
      switch (rowData.wholeApplication)
      {
      case QXNApplicationsModelPrivate::Auto: return tr("Force automatical");
      case QXNApplicationsModelPrivate::Manual: return tr("Force manual");
      case QXNApplicationsModelPrivate::Exclusion: return tr("Don't process");
      }
    }
    }
  }
  else if (role == Qt::CheckStateRole)
  {
    if (index.row() == 1)
      return (rowData.wholeApplication) ? Qt::Checked : Qt::Unchecked;
  }

  return QVariant();
}


void QXNApplicationsModel::load(const QStringList& manualApps, const QStringList& autoApps,
                                const QStringList& excludedApps, const QStringList& layoutRememberApps)
{
  QHash<QString,QXNApplicationsModelPrivate> result;

  foreach (const QString& manualApp, manualApps)
    result[manualApp] = QXNApplicationsModelPrivate(QXNApplicationsModelPrivate::Manual, manualApp);

  foreach (const QString& autoApp, autoApps)
    result[autoApp] = QXNApplicationsModelPrivate(QXNApplicationsModelPrivate::Auto, autoApp);

  foreach (const QString& excludedApp, excludedApps)
    result[excludedApp] = QXNApplicationsModelPrivate(QXNApplicationsModelPrivate::Exclusion, excludedApp);

  foreach (const QString& layoutRememberApp, layoutRememberApps)
  {
    if (result.contains(layoutRememberApp))
      result[layoutRememberApp].wholeApplication = true;
    else
      result[layoutRememberApp] = QXNApplicationsModelPrivate(QXNApplicationsModelPrivate::Default, layoutRememberApp,
                                                              true);
  }

  beginResetModel();
  m_data = result.values();
  endResetModel();
}
