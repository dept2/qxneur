#ifndef QXNAPPLICATIONSETTINGSDIALOG_H
#define QXNAPPLICATIONSETTINGSDIALOG_H

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
    QXNApplicationSettingsDialog(QWidget* parent=0);
    ~QXNApplicationSettingsDialog();

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
    Ui::QXNApplicationSettingsDialog* ui;
};

#endif // QXNAPPLICATIONSETTINGSDIALOG_H
