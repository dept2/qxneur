#ifndef ABBREVIATIONEDITDIALOG_H
#define ABBREVIATIONEDITDIALOG_H

#include <QDialog>
#include "ui_AbbreviationEditDialog.h"

class AbbreviationEditDialog : public QDialog, public Ui::AbbreviationEditDialog
{
  Q_OBJECT

  public:
    AbbreviationEditDialog(QWidget* parent=0);
    ~AbbreviationEditDialog();

    QString abbreviation() const;
    QString fullText() const;

  public slots:
    void setAbbreviation(const QString&);
    void setFullText(const QString&);

  protected slots:
    void verify();
};

#endif // ABBREVIATIONEDITDIALOG_H
