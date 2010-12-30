#ifndef APPLICATIONSETTINGSDIALOG_H
#define APPLICATIONSETTINGSDIALOG_H

#include <QDialog>
#include "ui_ApplicationSettingsDialog.h"

class ApplicationSettingsDialog : public QDialog, public Ui::ApplicationSettingsDialog
{
  Q_OBJECT

  public:
    ApplicationSettingsDialog(QWidget* parent=0);
    ~ApplicationSettingsDialog();

    QString windowName();
    bool storeLayout();

    enum LayoutSwitching
    {
      Default,
      Manual,
      Automatical,
      DontProcess
    };
    LayoutSwitching layoutSwithching();

  protected slots:
    void on_windowSelectButton_clicked();

    void verifyWindowName(const QString& name);
};

#endif // APPLICATIONSETTINGSDIALOG_H
