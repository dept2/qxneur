#ifndef QXNAPPLICATIONSMODEL_H
#define QXNAPPLICATIONSMODEL_H

// Qt
#include <QAbstractTableModel>

// Local
#include "QXNApplicationsModelPrivate.h"
#include "QXNApplicationSettingsDialog.h"


class QXNApplicationsModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    QXNApplicationsModel(QObject* parent = 0);
    virtual ~QXNApplicationsModel() { }

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool removeRow(int row, const QModelIndex& parent = QModelIndex());

    void load(const QStringList& manualApps, const QStringList& autoApps, const QStringList& excludedApps,
              const QStringList& layoutRememberApps);

    void addAppliaction(const QString& windowName, bool layoutRemember, LayoutSwitching::Mode layoutSwitching);

  private:
    QList<QXNApplicationsModelPrivate> m_data;
};

#endif // QXNAPPLICATIONSMODEL_H
