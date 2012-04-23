// C++ Implementation: QXNKeyboard
// Author: Boris Moiseev <moiseev@nii.voskhod.ru>, (C) 2009

// Local
#include "QXNKeyboard.h"
#include "QXNKeyboard_p.h"

// Qt
#include <QX11Info>

// X11
#include <X11/XKBlib.h>


QXNKeyboard::QXNKeyboard(QObject* parent)
  : QObject(parent), d_ptr(new QXNKeyboardPrivate)
{
  Q_D(QXNKeyboard);
  d->q_ptr = this;

  // check the XKB library version
  int major = XkbMajorVersion, minor = XkbMinorVersion;
  if (!XkbLibraryVersion(&major, &minor))
    qWarning("%s", qPrintable(tr("XKB header and library version does not match. The program may work incorrectly.")));

  // initialize the extension
  Display* dpy = QX11Info::display();
  int opcode, errorBase;
  bool xkbAvailable = XkbQueryExtension(dpy, &opcode, &d->_eventCode, &errorBase, &major, &minor);
  if (xkbAvailable)
  {
    // register for XKB events
    // group state change, i.e. the current group changed:
    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);
    // keyboard mapping change:
    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbMapNotify, XkbAllMapComponentsMask, XkbKeySymsMask);
    // group names change:
    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbNamesNotify, XkbAllNamesMask, XkbGroupNamesMask);
    // new keyboard:
    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbNewKeyboardNotify, XkbAllNewKeyboardEventsMask, XkbAllNewKeyboardEventsMask);

    // And load initial data
    d->reload();
  }
  else
  {
    qFatal("%s", qPrintable(tr("The X Server does not support a compatible XKB extension.")));
  }
}

QXNKeyboard::~QXNKeyboard()
{
  delete d_ptr;
}


int QXNKeyboard::groupCount()
{
  Q_D(QXNKeyboard);
  return d->_groupCount;
}

QList<QXNLanguage::Language> QXNKeyboard::groups()
{
  Q_D(QXNKeyboard);
  return d->_groups;
}


QXNLanguage::Language QXNKeyboard::currentGroup()
{
  Q_D(QXNKeyboard);
  return d->_groups[d->_currentGroup];
}


void QXNKeyboard::nextGroup()
{
  Q_D(QXNKeyboard);

  // Calculate the number of new group
  int newGroup = d->_currentGroup+1;
  if (newGroup >= d->_groupCount)
    newGroup = 0;

  // And apply it
  XkbLockGroup(QX11Info::display(), XkbUseCoreKbd, newGroup);
}


void QXNKeyboard::x11Event(XEvent* event)
{
  Q_D(QXNKeyboard);

  if (event->type == d->_eventCode)
  {
    // This an XKB event
    XkbEvent *xkb_ev = (XkbEvent *) event;

    if (xkb_ev->any.xkb_type == XkbStateNotify)
    {
      // state notify event, the current group has changed
      d->_currentGroup = xkb_ev->state.group;
      emit groupChanged(d->_groups[d->_currentGroup]);
    }
    else if (( (xkb_ev->any.xkb_type == XkbMapNotify) && (xkb_ev->map.changed & XkbKeySymsMask) )
          || ( (xkb_ev->any.xkb_type == XkbNamesNotify) && (xkb_ev->names.changed & XkbGroupNamesMask) )
          || (xkb_ev->any.xkb_type == XkbNewKeyboardNotify))
    {
      // keyboard layout has changed
      d->reload();
      emit layoutChanged();
    }
  }
}


// Declare the empty error handler to workaround the BadAtom error
extern "C" {
  static int IgnoreXError(Display*, XErrorEvent*) {
    return 0;
  }
}


QXNKeyboardPrivate::QXNKeyboardPrivate()
{
}

QXNKeyboardPrivate::~QXNKeyboardPrivate()
{
}


void QXNKeyboardPrivate::loadCurrentGroup()
{
  XkbStateRec rec;
  XkbGetState(QX11Info::display(), XkbUseCoreKbd, &rec);
  _currentGroup = rec.group;
}

void QXNKeyboardPrivate::loadGroupCount()
{
  // Initialize the XKB Descriptor
  XkbDescRec xkb;
  memset(&xkb, 0, sizeof(xkb));
  xkb.device_spec = XkbUseCoreKbd;

  // Get the number of keyboard groups
  XkbGetControls(QX11Info::display(), XkbGroupsWrapMask, &xkb);
  _groupCount = xkb.ctrls->num_groups;
  XkbFreeControls(&xkb, XkbGroupsWrapMask, 1);
}

void QXNKeyboardPrivate::loadGroupNames()
{
  // Initialize the XKB Descriptor
  XkbDescRec xkb;
  memset(&xkb, 0, sizeof(xkb));
  xkb.device_spec = XkbUseCoreKbd;

  // Store the display pointer
  Display* dpy = QX11Info::display();

  // Get the names of the currently configured keyboard groups
  char *names[XkbNumKbdGroups];
  XkbGetNames(dpy, XkbGroupNamesMask, &xkb);
  memset(names, 0, sizeof(char *) * XkbNumKbdGroups);

  // XGetAtomNames below may generate BadAtom error, which is not a problem.
  // (it may happen if the name for a group was not defined)
  // Thus we temporarily ignore X errors
  XErrorHandler oldHandler = XSetErrorHandler(IgnoreXError);
  XGetAtomNames(dpy, xkb.names->groups, _groupCount, names);
  // resume normal X error processing
  XSetErrorHandler(oldHandler);

  _groups.clear();
  for (int i = 0; i < _groupCount; ++i)
  {
    _groups.append(QXNLanguage::layoutNameToLanguage(QString::fromLocal8Bit(names[i])));
    if (names[i])
      XFree(names[i]);
  }
  XkbFreeNames(&xkb, XkbGroupNamesMask, 1);
}


void QXNKeyboardPrivate::reload()
{
  loadGroupCount();
  loadGroupNames();
  loadCurrentGroup();
}
