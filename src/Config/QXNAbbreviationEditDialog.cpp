#include "QXNAbbreviationEditDialog.h"

#include <QPushButton>
#include <QRegExpValidator>

QXNAbbreviationEditDialog::QXNAbbreviationEditDialog(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  abbreviationEdit->setValidator(new QRegExpValidator(QRegExp(QLatin1String("\\S*")), this));

  connect(abbreviationEdit, SIGNAL(textChanged(const QString&)), SLOT(verify()));
  connect(fullTextEdit, SIGNAL(textChanged(const QString&)), SLOT(verify()));
}

QXNAbbreviationEditDialog::~QXNAbbreviationEditDialog()
{}


QString QXNAbbreviationEditDialog::abbreviation() const
{
  return abbreviationEdit->text();
}

void QXNAbbreviationEditDialog::setAbbreviation(const QString& s)
{
  abbreviationEdit->setText(s);
}


QString QXNAbbreviationEditDialog::fullText() const
{
  return fullTextEdit->text();
}

void QXNAbbreviationEditDialog::setFullText(const QString& s)
{
  fullTextEdit->setText(s);
}


void QXNAbbreviationEditDialog::verify()
{
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled( !abbreviation().isEmpty() && !fullText().isEmpty() );
}
