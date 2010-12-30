#ifndef QXNAPPLICATIONSMODEL_H
#define QXNAPPLICATIONSMODEL_H

// Qt
#include <QAbstractTableModel>

// Local
#include "QXNApplicationsModelPrivate.h"


class QXNApplicationsModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    QXNApplicationsModel(QObject* parent = 0);
    virtual ~QXNApplicationsModel() { }

    virtual int columnCount(const QModelIndex& parent) const;
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex& index, int role) const;

    void load(const QStringList& manualApps, const QStringList& autoApps, const QStringList& excludedApps,
              const QStringList& layoutRememberApps);

  private:
    QList<QXNApplicationsModelPrivate> m_data;
};

#endif // QXNAPPLICATIONSMODEL_H
