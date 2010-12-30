#ifndef QXNCONFIGDIALOG_H
#define QXNCONFIGDIALOG_H

// Ui
#include "ui_QXNConfigDialog.h"

// Qt
#include <QDialog>

//Local
class QXNConfig;
class QXNApplicationsModel;


class QXNConfigDialog : public QDialog, public Ui::QXNConfigDialog
{
  Q_OBJECT

  public:
    QXNConfigDialog(QXNConfig* config, QWidget* parent = 0);
    ~QXNConfigDialog();

  public slots:
    void run();
    void accept();

  protected slots:
    void on_buttonBox_clicked(QAbstractButton*);

    // Applications
    void on_addApplicationButton_clicked();

    // Sounds
    void on_soundsTable_itemDoubleClicked(QTableWidgetItem*);
    void on_editSoundButton_clicked();

    // Abbreviations
    void on_addAbbreviationButton_clicked();
    void on_removeAbbreviationButton_clicked();
    void on_editAbbreviationButton_clicked();

    void abbreviationListChanged();

  protected:
    QXNConfig* m_xnconfig;

    void load();
    void save();

    int findAbbreviation(const QString&);
    bool replaceAbbreviationQuestion(const QString&);

  private:
    QXNApplicationsModel* m_appsModel;
};

#endif // QXNCONFIGDIALOG_H
