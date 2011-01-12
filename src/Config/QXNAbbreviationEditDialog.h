#ifndef QXNABBREVIATIONEDITDIALOG_H
#define QXNABBREVIATIONEDITDIALOG_H

#include <QDialog>
#include "ui_QXNAbbreviationEditDialog.h"

class QXNAbbreviationEditDialog : public QDialog, public Ui::QXNAbbreviationEditDialog
{
  Q_OBJECT

  public:
    QXNAbbreviationEditDialog(QWidget* parent=0);
    ~QXNAbbreviationEditDialog();

    QString abbreviation() const;
    QString fullText() const;

  public slots:
    void setAbbreviation(const QString&);
    void setFullText(const QString&);

  protected slots:
    void verify();
};

#endif // QXNABBREVIATIONEDITDIALOG_H
