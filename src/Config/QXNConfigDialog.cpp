// Class header
#include "QXNConfigDialog.h"

// Local components
#include "QXNConfig.h"
#include "ApplicationSettingsDialog.h"
#include "AbbreviationEditDialog.h"
#include "QXNApplicationsModel.h"

// Qt
#include <QMessageBox>
#include <QFileDialog>


QXNConfigDialog::QXNConfigDialog(QXNConfig* config, QWidget* parent)
  : QDialog(parent),
    m_xnconfig(config),
    m_appsModel(new QXNApplicationsModel(this))
{
  setupUi(this);
  customApplicationsView->setModel(m_appsModel);

  // Abbreviations
  connect(abbreviationTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
          SLOT(abbreviationListChanged()));
}


QXNConfigDialog::~QXNConfigDialog()
{ }


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
  manualModeCheckBox->setChecked(m_xnconfig->manualMode());
  selfEducateCheckBox->setChecked(m_xnconfig->autoEducate());
  dontProcessOnEnterCheckBox->setChecked(m_xnconfig->noProcessOnEnter());
  checkLanguageDuringInputCheckBox->setChecked(m_xnconfig->checkLanguageDuringInput());
  logInputCheckBox->setChecked(m_xnconfig->saveKeyboardLog());

  // Correction options
  correctAccidentalCapsLockCheckBox->setChecked(m_xnconfig->correctAccidentalCaps());
  correctTwoCapitalLettersCheckBox->setChecked(m_xnconfig->correctTwoCapitalLetters());
  correctSpaceWithPunctuationCheckBox->setChecked(m_xnconfig->correctSpaceWithPunctuation());
  disableCapsCheckBox->setChecked(m_xnconfig->disableCaps());

  // Advanced options
  saveSelectionCheckBox->setChecked(m_xnconfig->saveSelection());
  flushBuffersOnEnterCheckBox->setChecked(m_xnconfig->flushInternalBuffers());
  eventSendDelaySpinBox->setValue(m_xnconfig->eventSendDelay());
  logLevelCombo->setCurrentIndex(m_xnconfig->logLevel());

  // Sounds
  soundsGroupBox->setChecked(m_xnconfig->soundMode());
  for (int i=0; i<soundsTable->rowCount(); i++)
  {
    soundsTable->setItem(i, 1, new QTableWidgetItem(m_xnconfig->actionSound(i)));
  }

  // Abbreviations
  ignoreLayoutForAbbreviationsCheckBox->setChecked(m_xnconfig->ignoreLayoutForAbbreviations());
  StringToStringMap abbrList = m_xnconfig->abbreviations();

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

  // Applications
  m_appsModel->load(m_xnconfig->manualApps(), m_xnconfig->autoApps(), m_xnconfig->excludedApps(),
                    m_xnconfig->layoutRememberApps());
}


void QXNConfigDialog::save()
{
  // Working mode options
  m_xnconfig->setManualMode(manualModeCheckBox->isChecked());
  m_xnconfig->setAutoEducate(selfEducateCheckBox->isChecked());
  m_xnconfig->setNoProcessOnEnter(dontProcessOnEnterCheckBox->isChecked());
  m_xnconfig->setCheckLanguageDuringInput(checkLanguageDuringInputCheckBox->isChecked());
  m_xnconfig->setSaveKeyboardLog(logInputCheckBox->isChecked());

  // Correction options
  m_xnconfig->setCorrectAccidentalCaps(correctAccidentalCapsLockCheckBox->isChecked());
  m_xnconfig->setCorrectTwoCapitalLetters(correctTwoCapitalLettersCheckBox->isChecked());
  m_xnconfig->setCorrectSpaceWithPunctuation(correctSpaceWithPunctuationCheckBox->isChecked());
  m_xnconfig->setDisableCaps(disableCapsCheckBox->isChecked());

  // Advanced options
  m_xnconfig->setSaveSelection(saveSelectionCheckBox->isChecked());
  m_xnconfig->setFlushInternalBuffers(flushBuffersOnEnterCheckBox->isChecked());
  m_xnconfig->setEventSendDelay(eventSendDelaySpinBox->value());
  m_xnconfig->setLogLevel(logLevelCombo->currentIndex());

  // Sounds
  m_xnconfig->setSoundMode(soundsGroupBox->isChecked());
  for (int i = 0; i < soundsTable->rowCount(); ++i)
  {
    m_xnconfig->setActionSound(i, soundsTable->item(i, 1)->text());
  }

  // Abbreviations
  m_xnconfig->setIgnoreLayoutForAbbreviations(ignoreLayoutForAbbreviationsCheckBox->isChecked());
  StringToStringMap abbrList;
  for (int i = 0; i < abbreviationTable->rowCount(); ++i)
    abbrList[abbreviationTable->item(i, 0)->text()] = abbreviationTable->item(i, 1)->text();
  m_xnconfig->setAbbreviations(abbrList);

  m_xnconfig->save();
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
  int row = -1;
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
    {
      editDialog.abbreviationEdit->setFocus(Qt::OtherFocusReason);
    }
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
  bool itemExist = (abbreviationTable->rowCount() != 0);
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

  for (int i = 0; i < abbreviationTable->rowCount(); ++i)
    if (abbreviation == abbreviationTable->item(i, 0)->text())
    {
      ret = i;
      break;
    }

  return ret;
}


bool QXNConfigDialog::replaceAbbreviationQuestion(const QString& abbreviation)
{
  return QMessageBox::question(this, tr("QXNeur"),
                               tr("Abbreviation \"%1\" already exists. Do you want to replace it?").arg(abbreviation),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes;
}
