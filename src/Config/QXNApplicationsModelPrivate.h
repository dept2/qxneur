#ifndef QXNAPPLICATIONSMODELPRIVATE_H
#define QXNAPPLICATIONSMODELPRIVATE_H

#include "QXNApplicationsMode.h"


struct QXNApplicationsModelPrivate
{
  QXNApplicationsModelPrivate()
  {
    mode = LayoutSwitching::Default;
    wholeApplication = false;
  }

  QXNApplicationsModelPrivate(LayoutSwitching::Mode m, const QString& name, bool whole = false)
  {
    mode = m;
    appName = name;
    wholeApplication = whole;
  }

  ~QXNApplicationsModelPrivate() { };

  LayoutSwitching::Mode mode;
  QString appName;
  bool wholeApplication;
};

#endif // QXNAPPLICATIONSMODELPRIVATE_H
