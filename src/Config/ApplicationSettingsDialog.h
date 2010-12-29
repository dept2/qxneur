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
    bool dontProcess();
    bool storeLayout();

    enum LayoutSwitching
    {
      Default,
      Manual,
      Automatical
    };
    LayoutSwitching layoutSwithching();

  protected slots:
    void on_windowSelectButton_clicked();

    void verify();
};

#endif // APPLICATIONSETTINGSDIALOG_H
