// Class header
#include "QXNConfigDialog.h"

// Ui
#include "ui_QXNConfigDialog.h"

// Local
#include "QXNConfig.h"
#include "QXNApplicationSettingsDialog.h"
#include "QXNAbbreviationEditDialog.h"
#include "QXNApplicationsModel.h"
#include "QXNModelRoles.h"
#include "PropertyMapper.h"

// Qt
#include <QMessageBox>
#include <QFileDialog>


QXNConfigDialog::QXNConfigDialog(QXNConfig* config, QWidget* parent)
  : QDialog(parent),
    ui(new Ui::QXNConfigDialog),
    mapper(new PropertyMapper(this)),
    m_xnconfig(config),
    m_appsModel(new QXNApplicationsModel(this))
{
  ui->setupUi(this);
  ui->customApplicationsView->setModel(m_appsModel);

  ui->customApplicationsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui->customApplicationsView->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  ui->customApplicationsView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->customApplicationsView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->removeApplicationButton->setEnabled(false);

  connect(ui->customApplicationsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          SLOT(customApplicationsSelectionChanged()));

  // Abbreviations
  connect(ui->abbreviationTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
          SLOT(abbreviationListChanged()));

  // Mapping
  connect(mapper, SIGNAL(propertiesChanged(bool)),
          ui->buttonBox->button(QDialogButtonBox::Apply), SLOT(setEnabled(bool)));

  // Working mode options
  mapper->setMapping(ui->manualModeCheckBox, m_xnconfig, "manualMode");
  mapper->setMapping(ui->selfEducateCheckBox, m_xnconfig, "autoEducate");
  mapper->setMapping(ui->dontProcessOnEnterCheckBox, m_xnconfig, "noProcessOnEnter");
  mapper->setMapping(ui->checkLanguageDuringInputCheckBox, m_xnconfig, "checkLanguageDuringInput");
  mapper->setMapping(ui->logInputCheckBox, m_xnconfig, "saveKeyboardLog");
  mapper->setMapping(ui->autoCompletionCheckBox, m_xnconfig, "autoCompletion");

  // Correction options
  mapper->setMapping(ui->correctAccidentalCapsLockCheckBox, m_xnconfig, "correctAccidentalCaps");
  mapper->setMapping(ui->correctTwoCapitalLettersCheckBox, m_xnconfig, "correctTwoCapitalLetters");
  mapper->setMapping(ui->correctSpaceWithPunctuationCheckBox, m_xnconfig, "correctSpaceWithPunctuation");
  mapper->setMapping(ui->disableCapsCheckBox, m_xnconfig, "disableCaps");

  // Advanced options
  mapper->setMapping(ui->saveSelectionCheckBox, m_xnconfig, "saveSelectionAfterConvert");
  mapper->setMapping(ui->flushBuffersOnEnterCheckBox, m_xnconfig, "flushInternalBuffers");
  mapper->setMapping(ui->eventSendDelaySpinBox, m_xnconfig, "eventSendDelay");
  mapper->setMapping(ui->logLevelCombo, "currentIndex", m_xnconfig, "logLevel");

//  mapper->setMapping(, m_xnconfig, "");
}


QXNConfigDialog::~QXNConfigDialog()
{ }


void QXNConfigDialog::run()
{
  ui->listWidget->setCurrentRow(0);
  ui->configurationPagesStack->setCurrentIndex(0);
  ui->generalTabWidget->setCurrentIndex(0);
  ui->soundsTable->setCurrentCell(0, 1);
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
  if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    save();
}


void QXNConfigDialog::load()
{
  // Sounds
  ui->soundsGroupBox->setChecked(m_xnconfig->soundMode());
  for (int i = 0; i < ui->soundsTable->rowCount(); i++)
  {
    ui->soundsTable->setItem(i, 1, new QTableWidgetItem(m_xnconfig->actionSound(i)));
  }

  // Abbreviations
  ui->ignoreLayoutForAbbreviationsCheckBox->setChecked(m_xnconfig->ignoreLayoutForAbbreviations());
  StringToStringMap abbrList = m_xnconfig->abbreviations();

  // Fill the abbreviation table
  ui->abbreviationTable->setRowCount(abbrList.size());
  QMapIterator<QString,QString> it(abbrList);
  int i = -1;
  while (it.hasNext())
  {
    it.next();
    ++i;

    ui->abbreviationTable->setItem(i, 0, new QTableWidgetItem(it.key()));
    ui->abbreviationTable->setItem(i, 1, new QTableWidgetItem(it.value()));
  }
  abbreviationListChanged();

  // Applications
  m_appsModel->load(m_xnconfig->manualApps(), m_xnconfig->autoApps(), m_xnconfig->excludedApps(),
                    m_xnconfig->layoutRememberApps());

  mapper->load();
}


void QXNConfigDialog::save()
{
  // Sounds
  m_xnconfig->setSoundMode(ui->soundsGroupBox->isChecked());
  for (int i = 0; i < ui->soundsTable->rowCount(); ++i)
  {
    m_xnconfig->setActionSound(i, ui->soundsTable->item(i, 1)->text());
  }

  // Abbreviations
  m_xnconfig->setIgnoreLayoutForAbbreviations(ui->ignoreLayoutForAbbreviationsCheckBox->isChecked());
  StringToStringMap abbrList;
  for (int i = 0; i < ui->abbreviationTable->rowCount(); ++i)
    abbrList[ui->abbreviationTable->item(i, 0)->text()] = ui->abbreviationTable->item(i, 1)->text();
  m_xnconfig->setAbbreviations(abbrList);

  mapper->apply();
  m_xnconfig->save();
}


void QXNConfigDialog::on_addApplicationButton_clicked()
{
  QXNApplicationSettingsDialog settingsDialog(this);

  if (settingsDialog.exec() == QDialog::Accepted)
  {
    QString windowName = settingsDialog.windowName();

    switch (settingsDialog.layoutSwithching())
    {
    case LayoutSwitching::Automatical:
      m_xnconfig->setAutoApps(m_xnconfig->autoApps() << windowName);
      break;
    case LayoutSwitching::Manual:
      m_xnconfig->setManualApps(m_xnconfig->manualApps() << windowName);
      break;
    case LayoutSwitching::DontProcess:
      m_xnconfig->setExcludedApps(m_xnconfig->excludedApps() << windowName);
      break;
    default:
      break;
    }

    if (settingsDialog.storeLayout())
      m_xnconfig->setLayoutRememberApps(m_xnconfig->layoutRememberApps() << windowName);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

    m_appsModel->load(m_xnconfig->manualApps(), m_xnconfig->autoApps(), m_xnconfig->excludedApps(),
                      m_xnconfig->excludedApps());
  }
}


void QXNConfigDialog::on_removeApplicationButton_clicked()
{
  int row = ui->customApplicationsView->currentIndex().row();

  bool wholeApplication = m_appsModel->data(m_appsModel->index(row, 1), QXNModelRoles::DataRole).toBool();
  LayoutSwitching::Mode mode = static_cast<LayoutSwitching::Mode>(m_appsModel->data(m_appsModel->index(row, 2),
                                                               QXNModelRoles::DataRole).toInt());
  QString application = m_appsModel->data(m_appsModel->index(row, 0)).toString();

  switch (mode)
  {
  case LayoutSwitching::Automatical:
  {
    QStringList autoApps = m_xnconfig->autoApps();
    autoApps.removeOne(application);
    m_xnconfig->setAutoApps(autoApps);
  }
  break;
  case LayoutSwitching::Manual:
  {
    QStringList manualApps = m_xnconfig->manualApps();
    manualApps.removeOne(application);
    m_xnconfig->setManualApps(manualApps);
  }
  break;
  case LayoutSwitching::DontProcess:
  {
    QStringList dontProcessApps = m_xnconfig->excludedApps();
    dontProcessApps.removeOne(application);
    m_xnconfig->setExcludedApps(dontProcessApps);
  }
  default: break;
  }

  if (wholeApplication)
  {
    QStringList wholeApplications = m_xnconfig->layoutRememberApps();
    wholeApplications.removeOne(application);
    m_xnconfig->setLayoutRememberApps(wholeApplications);
  }

  m_appsModel->removeRow(row);
  ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

  m_appsModel->load(m_xnconfig->manualApps(), m_xnconfig->autoApps(), m_xnconfig->excludedApps(),
                    m_xnconfig->layoutRememberApps());
}


void QXNConfigDialog::on_soundsTable_itemDoubleClicked(QTableWidgetItem* item)
{
  ui->soundsTable->editItem(ui->soundsTable->item(item->row(), 1));
}


void QXNConfigDialog::on_editSoundButton_clicked()
{
  QTableWidgetItem* current = ui->soundsTable->item(ui->soundsTable->currentRow(), 1);
  QString fileName = current->text();
  if (!fileName.isEmpty())
    fileName.left(fileName.lastIndexOf(QChar::fromAscii('/')));
  else
    fileName = QString();

  fileName = QFileDialog::getOpenFileName(this, QString(), fileName);
  current->setText(fileName);
}


void QXNConfigDialog::on_addAbbreviationButton_clicked()
{
  QXNAbbreviationEditDialog editDialog(this);

  bool stopFlag = false;
  int row = -1;
  while (!stopFlag && (editDialog.exec() == QDialog::Accepted))
  {
    // Try to find the item with identical abbreviation
    int exist = findAbbreviation(editDialog.abbreviation());
    // This abbreviation does not exist
    if (exist == -1)
    {
      row = ui->abbreviationTable->rowCount();
      ui->abbreviationTable->insertRow(row);

      // Add the item to table
      ui->abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
      ui->abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

      stopFlag = true;
    }
    // Abbreviation exists, ask user what to do
    else if (replaceAbbreviationQuestion(editDialog.abbreviation()))
    {
      // Add the item to table
      ui->abbreviationTable->setItem(exist, 0, new QTableWidgetItem(editDialog.abbreviation()));
      ui->abbreviationTable->setItem(exist, 1, new QTableWidgetItem(editDialog.fullText()));

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
    ui->abbreviationTable->setCurrentCell(row, 0);

    // Refresh the buttons
    abbreviationListChanged();
  }
}


void QXNConfigDialog::on_removeAbbreviationButton_clicked()
{
  int row = ui->abbreviationTable->currentRow();
  Q_ASSERT(row >= 0);

  // Remove row
  ui->abbreviationTable->removeRow(row);

  // Set the cursor to the last nearest item after deletion
  if (row < ui->abbreviationTable->rowCount())
    ui->abbreviationTable->setCurrentCell(row, 0);
  else if (ui->abbreviationTable->rowCount() > 0)
    ui->abbreviationTable->setCurrentCell(row - 1, 0);

  // Refresh the buttons
  abbreviationListChanged();
}


void QXNConfigDialog::on_editAbbreviationButton_clicked()
{
  int row = ui->abbreviationTable->currentRow();
  Q_ASSERT(row >= 0);

  // Construct the dialog
  QXNAbbreviationEditDialog editDialog(this);
  editDialog.setAbbreviation(ui->abbreviationTable->item(row, 0)->text());
  editDialog.setFullText(ui->abbreviationTable->item(row, 1)->text());

  bool stopFlag = false;
  while (!stopFlag && (editDialog.exec() == QDialog::Accepted))
  {
    // Only the full text may have been changed
    if (ui->abbreviationTable->item(row, 0)->text() == editDialog.abbreviation())
    {
      // Change the items if dialog is accepted
      ui->abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
      ui->abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

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
        ui->abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
        ui->abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

        stopFlag = true;
      }
      // Found an identical abbreviation, need to ask user
      else if(replaceAbbreviationQuestion(editDialog.abbreviation()))
      {
        // User told to replace, let's do what we can
        ui->abbreviationTable->removeRow(exist);
        row = ui->abbreviationTable->currentRow();
        ui->abbreviationTable->setItem(row, 0, new QTableWidgetItem(editDialog.abbreviation()));
        ui->abbreviationTable->setItem(row, 1, new QTableWidgetItem(editDialog.fullText()));

        stopFlag = true;
      }
      else
      {
        editDialog.abbreviationEdit->setFocus(Qt::OtherFocusReason);
      }
    }
  }

  // If the item was changed
  if (stopFlag)
  {
    // Select the added item
    ui->abbreviationTable->setCurrentCell(row, 0);

    // Refresh the buttons
    abbreviationListChanged();
  }
}


void QXNConfigDialog::abbreviationListChanged()
{
  bool itemExist = (ui->abbreviationTable->rowCount() != 0);
  bool itemSelected = (ui->abbreviationTable->currentRow() >= 0);

  // Remove
  ui->removeAbbreviationButton->setVisible(itemExist);
  ui->removeAbbreviationButton->setEnabled(itemSelected);

  // Edit
  ui->editAbbreviationButton->setVisible(itemExist);
  ui->editAbbreviationButton->setEnabled(itemSelected);
}


int QXNConfigDialog::findAbbreviation(const QString& abbreviation)
{
  int ret = -1;

  for (int i = 0; i < ui->abbreviationTable->rowCount(); ++i)
    if (abbreviation == ui->abbreviationTable->item(i, 0)->text())
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


void QXNConfigDialog::customApplicationsSelectionChanged()
{
  bool empty = ui->customApplicationsView->selectionModel()->selectedRows().isEmpty();
  ui->removeApplicationButton->setDisabled(empty);
  ui->editApplicationButton->setDisabled(empty);
}


void QXNConfigDialog::on_editApplicationButton_clicked()
{
  int row = ui->customApplicationsView->currentIndex().row();

  const QString& application = m_appsModel->data(m_appsModel->index(row, 0)).toString();
  bool wholeApplication = m_appsModel->data(m_appsModel->index(row, 1), QXNModelRoles::DataRole).toBool();
  LayoutSwitching::Mode mode = static_cast<LayoutSwitching::Mode>(m_appsModel->data(m_appsModel->index(row, 2),
                                                                               QXNModelRoles::DataRole).toInt());

  QXNApplicationSettingsDialog settingsDialog(this);
  settingsDialog.setData(application, wholeApplication, mode);

  if (settingsDialog.exec() == QDialog::Accepted)
  {
    QString windowName = settingsDialog.windowName();

    QStringList apps = m_xnconfig->autoApps();
    if (apps.removeOne(application))
      m_xnconfig->setAutoApps(apps);

    apps = m_xnconfig->manualApps();
    if (apps.removeOne(application))
      m_xnconfig->setManualApps(apps);

    apps = m_xnconfig->excludedApps();
    if (apps.removeOne(application))
      m_xnconfig->setExcludedApps(apps);

    if (wholeApplication)
    {
      apps = m_xnconfig->layoutRememberApps();
      apps.removeOne(application);
      m_xnconfig->setLayoutRememberApps(apps);
    }

    switch (settingsDialog.layoutSwithching())
    {
    case LayoutSwitching::Automatical:
      m_xnconfig->setAutoApps(m_xnconfig->autoApps() << windowName);
      break;
    case LayoutSwitching::Manual:
      m_xnconfig->setManualApps(m_xnconfig->manualApps() << windowName);
      break;
    case LayoutSwitching::DontProcess:
      m_xnconfig->setExcludedApps(m_xnconfig->excludedApps() << windowName);
      break;
    default:
      break;
    }

    if (settingsDialog.storeLayout())
      m_xnconfig->setLayoutRememberApps(m_xnconfig->layoutRememberApps() << windowName);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

    m_appsModel->load(m_xnconfig->manualApps(), m_xnconfig->autoApps(), m_xnconfig->excludedApps(),
                      m_xnconfig->layoutRememberApps());
  }
}

