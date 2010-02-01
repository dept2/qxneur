#include "AbbreviationEditDialog.h"

#include <QPushButton>
#include <QRegExpValidator>

AbbreviationEditDialog::AbbreviationEditDialog(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  abbreviationEdit->setValidator(new QRegExpValidator(QRegExp("\\S*"), this));

  connect(abbreviationEdit, SIGNAL(textChanged(const QString&)), SLOT(verify()));
  connect(fullTextEdit, SIGNAL(textChanged(const QString&)), SLOT(verify()));
}

AbbreviationEditDialog::~AbbreviationEditDialog()
{}


QString AbbreviationEditDialog::abbreviation() const
{
  return abbreviationEdit->text();
}

void AbbreviationEditDialog::setAbbreviation(const QString& s)
{
  abbreviationEdit->setText(s);
}


QString AbbreviationEditDialog::fullText() const
{
  return fullTextEdit->text();
}

void AbbreviationEditDialog::setFullText(const QString& s)
{
  fullTextEdit->setText(s);
}


void AbbreviationEditDialog::verify()
{
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled( !abbreviation().isEmpty() && !fullText().isEmpty() );
}
