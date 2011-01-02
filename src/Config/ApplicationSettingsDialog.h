#ifndef APPLICATIONSETTINGSDIALOG_H
#define APPLICATIONSETTINGSDIALOG_H

// Qt
#include <QDialog>

// Ui
namespace Ui
{
  class ApplicationSettingsDialog;
};


class ApplicationSettingsDialog : public QDialog
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

  private slots:
    void on_windowSelectButton_clicked();

    void verify();

  private:
    Ui::ApplicationSettingsDialog* ui;
};

#endif // APPLICATIONSETTINGSDIALOG_H
