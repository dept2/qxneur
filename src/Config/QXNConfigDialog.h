#ifndef QXNCONFIGDIALOG_H
#define QXNCONFIGDIALOG_H

// Ui
namespace Ui
{
  class QXNConfigDialog;
};

// Qt
#include <QDialog>
class QAbstractButton;
class QTableWidgetItem;

// Local
class QXNConfig;
class QXNApplicationsModel;
class PropertyMapper;


class QXNConfigDialog : public QDialog
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
    void on_removeApplicationButton_clicked();

    // Sounds
    void on_soundsTable_itemDoubleClicked(QTableWidgetItem*);
    void on_editSoundButton_clicked();

    // Abbreviations
    void on_addAbbreviationButton_clicked();
    void on_removeAbbreviationButton_clicked();
    void on_editAbbreviationButton_clicked();

    void abbreviationListChanged();

    void customApplicationsSelectionChanged();

  private:
    Ui::QXNConfigDialog* ui;
    PropertyMapper* mapper;
    QXNConfig* m_xnconfig;
    QXNApplicationsModel* m_appsModel;

    void load();
    void save();

    int findAbbreviation(const QString&);
    bool replaceAbbreviationQuestion(const QString&);
};

#endif // QXNCONFIGDIALOG_H
