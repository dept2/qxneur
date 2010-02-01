// Local
#include "ApplicationSettingsDialog.h"

// Qt
#include <QX11Info>
#include <QPushButton>

// X11
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xmu/WinUtil.h>


ApplicationSettingsDialog::ApplicationSettingsDialog(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  connect(windowNameEdit, SIGNAL(textChanged(const QString&)), SLOT(verify()));
  connect(dontProcessCheckBox, SIGNAL(toggled(bool)), SLOT(verify()));
  connect(storeLayoutCheckBox, SIGNAL(toggled(bool)), SLOT(verify()));
  connect(layoutSwitchingGroup, SIGNAL(toggled(bool)), SLOT(verify()));

  verify();
}

ApplicationSettingsDialog::~ApplicationSettingsDialog()
{}


Window selectWindow(Display *dpy)
{
  Cursor cursor = XCreateFontCursor(dpy, XC_crosshair);

  Window root = RootWindow(dpy, 0);
  XGrabPointer(dpy, root, False, ButtonPressMask|ButtonReleaseMask,
               GrabModeSync, GrabModeAsync, root, cursor, CurrentTime);

  Window target_win = None;
  int buttons = 0;
  while ((target_win == None) || (buttons != 0))
  {
    XEvent event;
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type)
    {
      case ButtonPress:
        if (target_win == None)
        {
          target_win = event.xbutton.subwindow;
          if (target_win == None)
            target_win = root;
        }
        buttons++;
        break;
      case ButtonRelease:
        if (buttons > 0)
          buttons--;
        break;
    }
  }

  XUngrabPointer(dpy, CurrentTime);
  return XmuClientWindow(dpy, target_win);
}

void ApplicationSettingsDialog::on_windowSelectButton_clicked()
{
  Display* display = QX11Info::display();
  Window window = selectWindow(display);

  XClassHint* classHint = XAllocClassHint();

  if (XGetClassHint(display, window, classHint))
    windowNameEdit->setText(QString(classHint->res_class));

  XFree(classHint);
}


void ApplicationSettingsDialog::verify()
{
  bool e = !windowNameEdit->text().isEmpty() && ( dontProcessCheckBox->isChecked() ||
                                                  (storeLayoutCheckBox->isVisible() && storeLayoutCheckBox->isChecked() ) ||
                                                  layoutSwitchingGroup->isChecked() );
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled( e );
}


QString ApplicationSettingsDialog::windowName()
{
  return windowNameEdit->text();
}

bool ApplicationSettingsDialog::dontProcess()
{
  return dontProcessCheckBox->isChecked();
}

bool ApplicationSettingsDialog::storeLayout()
{
  return storeLayoutCheckBox->isChecked();
}

ApplicationSettingsDialog::LayoutSwitching ApplicationSettingsDialog::layoutSwithching()
{
  if (layoutSwitchingGroup->isChecked())
  {
    if (forceAutomaticalRadioButton->isChecked())
      return Automatical;
    else
      return Manual;
  }
  else
    return Default;
}
