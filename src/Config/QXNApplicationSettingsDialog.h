#ifndef QXNAPPLICATIONSETTINGSDIALOG_H
#define QXNAPPLICATIONSETTINGSDIALOG_H

// Local
#include "QXNApplicationsMode.h"

// Qt
#include <QDialog>

// Ui
namespace Ui
{
  class QXNApplicationSettingsDialog;
};


class QXNApplicationSettingsDialog : public QDialog
{
  Q_OBJECT

  public:
    QXNApplicationSettingsDialog(QWidget* parent = 0);
    ~QXNApplicationSettingsDialog();

    QString windowName();
    bool storeLayout();
    LayoutSwitching::Mode layoutSwithching();

    void setData(const QString& application, bool wholeApplication, LayoutSwitching::Mode mode);

  private slots:
    void on_windowSelectButton_clicked();

    void verify();

  private:
    Ui::QXNApplicationSettingsDialog* ui;
};

#endif // QXNAPPLICATIONSETTINGSDIALOG_H
