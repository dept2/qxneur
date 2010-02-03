// Class header
#include "QXNConfigDialog.h"

// Local components
#include "QXNConfig.h"
#include "ApplicationSettingsDialog.h"
#include "AbbreviationEditDialog.h"

// Qt
#include <QMessageBox>
#include <QFileDialog>


QXNConfigDialog::QXNConfigDialog(QXNConfig* config, QWidget* parent)
    : QDialog(parent), xnconfig(config)
{
  setupUi(this);

  // Abbreviations
  connect(abbreviationTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
          SLOT(abbreviationListChanged()));
}

QXNConfigDialog::~QXNConfigDialog()
{
}


void QXNConfigDialog::run()
{
  listWidget->setCurrentRow(0);
  configurationPagesStack->setCurrentIndex(0);
  generalTabWidget->setCurrentIndex(0);
  soundsTable->setCurrentCell(0, 1);
  load();
  show();
}


void QXNConfigDialog::accept()
{
  save();
  QDialog::accept();
}

void QXNConfigDialog::on_buttonBox_clicked(QAbstractButton* button)
{
  // Apply button clicked
  if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    save();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QXNConfigDialog::load()
{
  // Working mode options
  manualModeCheckBox->setChecked(xnconfig->manualMode());
  selfEducateCheckBox->setChecked(xnconfig->autoEducate());
  dontProcessOnEnterCheckBox->setChecked(xnconfig->noProcessOnEnter());
  checkLanguageDuringInputCheckBox->setChecked(xnconfig->checkLanguageDuringInput());
  logInputCheckBox->setChecked(xnconfig->saveKeyboardLog());

  // Correction options
  correctAccidentalCapsLockCheckBox->setChecked(xnconfig->correctAccidentalCaps());
  correctTwoCapitalLettersCheckBox->setChecked(xnconfig->correctTwoCapitalLetters());
  correctSpaceWithPunctuationCheckBox->setChecked(xnconfig->correctSpaceWithPunctuation());
  disableCapsCheckBox->setChecked(xnconfig->disableCaps());

  // Advanced options
  saveSelectionCheckBox->setChecked(xnconfig->saveSelection());
  flushBuffersOnEnterCheckBox->setChecked(xnconfig->flushInternalBuffers());
  eventSendDelaySpinBox->setValue(xnconfig->eventSendDelay());
  logLevelCombo->setCurrentIndex(xnconfig->logLevel());

  // Sounds
  soundsGroupBox->setChecked(xnconfig->soundMode());
  for (int i=0; i<soundsTable->rowCount(); i++)
  {
    soundsTable->setItem(i, 1, new QTableWidgetItem(xnconfig->actionSound(i)));
  }

  // Abbreviations
  ignoreLayoutForAbbreviationsCheckBox->setChecked(xnconfig->ignoreLayoutForAbbreviations());
  StringToStringMap abbrList = xnconfig->abbreviations();

  // Fill the abbreviation table
  abbreviationTable->setRowCount(abbrList.size());
  QMapIterator<QString,QString> it(abbrList);
  int i = -1;
  while (it.hasNext())
  {
    it.next();
    ++i;

    abbreviationTable->setItem(i, 0, new QTableWidgetItem(it.key()));
    abbreviationTable->setItem(i, 1, new QTableWidgetItem(it.value()));
  }
  abbreviationListChanged();
}

void QXNConfigDialog::save()
{
  // Working mode options
  xnconfig->setManualMode(manualModeCheckBox->isChecked());
  xnconfig->setAutoEducate(selfEducateCheckBox->isChecked());
  xnconfig->setNoProcessOnEnter(dontProcessOnEnterCheckBox->isChecked());
  xnconfig->setCheckLanguageDuringInput(checkLanguageDuringInputCheckBox->isChecked());
  xnconfig->setSaveKeyboardLog(logInputCheckBox->isChecked());

  // Correction options
  xnconfig->setCorrectAccidentalCaps(correctAccidentalCapsLockCheckBox->isChecked());
  xnconfig->setCorrectTwoCapitalLetters(correctTwoCapitalLettersCheckBox->isChecked());
  xnconfig->setCorrectSpaceWithPunctuation(correctSpaceWithPunctuationCheckBox->isChecked());
  xnconfig->setDisableCaps(disableCapsCheckBox->isChecked());

  // Advanced options
  xnconfig->setSaveSelection(saveSelectionCheckBox->isChecked());
  xnconfig->setFlushInternalBuffers(flushBuffersOnEnterCheckBox->isChecked());
  xnconfig->setEventSendDelay(eventSendDelaySpinBox->value());
  xnconfig->setLogLevel(logLevelCombo->currentIndex());

  // Sounds
  xnconfig->setSoundMode(soundsGroupBox->isChecked());
  for (int i=0; i<soundsTable->rowCount(); i++)
  {
    xnconfig->setActionSound(i, soundsTable->item(i, 1)->text());
  }

  // Abbreviations
  xnconfig->setIgnoreLayoutForAbbreviations(ignoreLayoutForAbbreviationsCheckBox->isChecked());
  StringToStringMap abbrList;
  for (int i=0; i<abbreviationTable->rowCount(); i++)
    abbrList[abbreviationTable->item(i, 0)->text()] = abbreviationTable->item(i, 1)->text();
  xnconfig->setAbbreviations(abbrList);

  xnconfig->save();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QXNConfigDialog::on_addApplicationButton_clicked()
{
  ApplicationSettingsDialog settingsDialog(this);

  if (settingsDialog.exec() == QDialog::Accepted)
  {
    qDebug("accepted");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QXNConfigDialog::on_soundsTable_itemDoubleClicked(QTableWidgetItem* item)
{
  soundsTable->editItem(soundsTable->item(item->row(), 1));
}

void QXNConfigDialog::on_editSoundButton_clicked()
{
  QTableWidgetItem* current = soundsTable->item(soundsTable->currentRow(), 1);
  QString fileName = current->text();
  if (!fileName.isEmpty())
  {
    fileName.left(fileName.lastIndexOf(QChar::fromAscii('/')));
  }
  else
    fileName = QString();

  fileName = QFileDialog::getOpenFileName(this, QString(), fileName);
  current->setText(fileName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QXNConfigDialog::on_addAbbreviationButton_clicked()
{
  AbbreviationEditDialog editDialog(this);

  bool stopFlag = false;
  int row=-1;
  while (!stopFlag && (editDialog.exec() == QDialog::Accepted))
  {
    // Try to find the item with identical abbreviation
    int exist = findAbbreviation(editDialog.abbreviation());
    // This abbreviation does not exist
    if (exist == -1)
    {
      row = abbreviationTable->rowCount();
      abbreviationTable->insertRow(row);

      // Add the item to table
      abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
      abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

      stopFlag = true;
    }
    // Abbreviation exists, ask user what to do
    else if (replaceAbbreviationQuestion(editDialog.abbreviation()))
    {
      // Add the item to table
      abbreviationTable->setItem(exist, 0, new QTableWidgetItem(editDialog.abbreviation()));
      abbreviationTable->setItem(exist, 1, new QTableWidgetItem(editDialog.fullText()));

      stopFlag = true;
    }
    else
      editDialog.abbreviationEdit->setFocus(Qt::OtherFocusReason);
  }

  // If the item was added (or replaced)
  if (stopFlag)
  {
    // Select the added item
    abbreviationTable->setCurrentCell(row, 0);

    // Refresh the buttons
    abbreviationListChanged();
  }
}

void QXNConfigDialog::on_removeAbbreviationButton_clicked()
{
  int row = abbreviationTable->currentRow();
  Q_ASSERT(row >= 0);

  // Remove row
  abbreviationTable->removeRow(row);

  // Set the cursor to the last nearest item after deletion
  if (row < abbreviationTable->rowCount())
    abbreviationTable->setCurrentCell(row, 0);
  else if (abbreviationTable->rowCount() > 0)
    abbreviationTable->setCurrentCell(row-1, 0);

  // Refresh the buttons
  abbreviationListChanged();
}

void QXNConfigDialog::on_editAbbreviationButton_clicked()
{
  int row = abbreviationTable->currentRow();
  Q_ASSERT(row >= 0);

  // Construct the dialog
  AbbreviationEditDialog editDialog(this);
  editDialog.setAbbreviation(abbreviationTable->item(row, 0)->text());
  editDialog.setFullText(abbreviationTable->item(row, 1)->text());

  bool stopFlag = false;
  while (!stopFlag && (editDialog.exec() == QDialog::Accepted))
  {
    // Only the full text may have been changed
    if (abbreviationTable->item(row, 0)->text() == editDialog.abbreviation())
    {
      // Change the items if dialog is accepted
      abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
      abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

      stopFlag = true;
    }
    // Abbreviation changed
    else
    {
      // Try to find the identical abbreviation
      int exist = findAbbreviation(editDialog.abbreviation());

      // No identical abbreviations found, just change the item
      if (exist == -1)
      {
        abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
        abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

        stopFlag = true;
      }
      // Found an identical abbreviation, need to ask user
      else if(replaceAbbreviationQuestion(editDialog.abbreviation()))
      {
        // User told to replace, let's do what we can
        abbreviationTable->removeRow(exist);
        row = abbreviationTable->currentRow();
        abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
        abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

        stopFlag = true;
      }
      else
        editDialog.abbreviationEdit->setFocus(Qt::OtherFocusReason);
    }
  }

  // If the item was changed
  if (stopFlag)
  {
    // Select the added item
    abbreviationTable->setCurrentCell(row, 0);

    // Refresh the buttons
    abbreviationListChanged();
  }
}

void QXNConfigDialog::abbreviationListChanged()
{
  bool itemExist = (abbreviationTable->rowCount()!=0);
  bool itemSelected = (abbreviationTable->currentRow() >= 0);

  // Remove
  removeAbbreviationButton->setVisible(itemExist);
  removeAbbreviationButton->setEnabled(itemSelected);

  // Edit
  editAbbreviationButton->setVisible(itemExist);
  editAbbreviationButton->setEnabled(itemSelected);
}

int QXNConfigDialog::findAbbreviation(const QString& abbreviation)
{
  int ret=-1;

  for (int i=0; i<abbreviationTable->rowCount(); i++)
    if (abbreviation == abbreviationTable->item(i, 0)->text())
    {
      ret = i;
      break;
    }

  return ret;
}

bool QXNConfigDialog::replaceAbbreviationQuestion(const QString& abbreviation)
{
  return QMessageBox::question(this,
                               tr("QXNeur"),
                               tr("Abbreviation \"%1\" already exists. Do you want to replace it?").arg(abbreviation),
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::No)
        == QMessageBox::Yes;
}
