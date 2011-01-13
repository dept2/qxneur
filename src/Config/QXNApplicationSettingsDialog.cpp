// Local
#include "QXNApplicationSettingsDialog.h"

// Qt
#include <QX11Info>
#include <QPushButton>

// Ui
#include "ui_QXNApplicationSettingsDialog.h"

// X11
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xmu/WinUtil.h>


QXNApplicationSettingsDialog::QXNApplicationSettingsDialog(QWidget* parent)
  : QDialog(parent),
    ui(new Ui::QXNApplicationSettingsDialog)
{
  ui->setupUi(this);

  ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

  connect(ui->windowNameEdit, SIGNAL(textChanged(QString)), SLOT(verify()));
  connect(ui->storeLayoutCheckBox, SIGNAL(toggled(bool)), SLOT(verify()));
  connect(ui->layoutSwitchingGroup, SIGNAL(toggled(bool)), SLOT(verify()));
}


QXNApplicationSettingsDialog::~QXNApplicationSettingsDialog()
{ }


Window selectWindow(Display* dpy)
{
  Cursor cursor = XCreateFontCursor(dpy, XC_crosshair);

  Window root = RootWindow(dpy, 0);
  XGrabPointer(dpy, root, False, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, root, cursor,
               CurrentTime);

  Window target_win = None;
  int buttons = 0;
  while ((target_win == None) || (buttons != 0))
  {
    XEvent event;
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask | ButtonReleaseMask, &event);
    switch (event.type)
    {
      case ButtonPress:
        if (target_win == None)
        {
          target_win = event.xbutton.subwindow;
          if (target_win == None)
            target_win = root;
        }
        ++buttons;
        break;
      case ButtonRelease:
        if (buttons > 0)
          --buttons;
        break;
    }
  }

  XUngrabPointer(dpy, CurrentTime);
  return XmuClientWindow(dpy, target_win);
}


void QXNApplicationSettingsDialog::on_windowSelectButton_clicked()
{
  Display* display = QX11Info::display();
  Window window = selectWindow(display);

  XClassHint* classHint = XAllocClassHint();

  if (XGetClassHint(display, window, classHint))
    ui->windowNameEdit->setText(QString::fromLocal8Bit(classHint->res_class));

  XFree(classHint);
}


void QXNApplicationSettingsDialog::verify()
{
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->windowNameEdit->text().isEmpty()
                                                           && (ui->storeLayoutCheckBox->isChecked()
                                                               || ui->layoutSwitchingGroup->isChecked()));
}


QString QXNApplicationSettingsDialog::windowName()
{
  return ui->windowNameEdit->text();
}


bool QXNApplicationSettingsDialog::storeLayout()
{
  return ui->storeLayoutCheckBox->isChecked();
}


LayoutSwitching::Mode QXNApplicationSettingsDialog::layoutSwithching()
{
  if (ui->layoutSwitchingGroup->isChecked())
  {
    if (ui->forceAutomaticalRadioButton->isChecked())
      return LayoutSwitching::Automatical;
    else if (ui->forceManualRadioButton->isChecked())
      return LayoutSwitching::Manual;
    else
      return LayoutSwitching::DontProcess;
  }

  return LayoutSwitching::Default;
}


void QXNApplicationSettingsDialog::setData(const QString& application, bool wholeApplication, LayoutSwitching::Mode mode)
{
  ui->windowNameEdit->setText(application);
  ui->storeLayoutCheckBox->setChecked(wholeApplication);

  if (mode != LayoutSwitching::Default)
  {
    ui->layoutSwitchingGroup->setChecked(true);

    switch (mode)
    {
    case LayoutSwitching::Automatical:
      ui->forceAutomaticalRadioButton->setChecked(true);
      break;
    case LayoutSwitching::Manual:
      ui->forceManualRadioButton->setChecked(true);
      break;
    case LayoutSwitching::DontProcess:
      ui->dontProcessRadioButton->setChecked(true);
      break;
    default: break;
    }
  }
}
