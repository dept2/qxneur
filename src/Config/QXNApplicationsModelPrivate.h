#ifndef QXNAPPLICATIONSMODELPRIVATE_H
#define QXNAPPLICATIONSMODELPRIVATE_H

struct QXNApplicationsModelPrivate
{
  enum Mode
  {
    Default,
    Auto,
    Manual,
    Exclusion
  };

  QXNApplicationsModelPrivate()
  {
    mode = Default;
    wholeApplication = false;
  }

  QXNApplicationsModelPrivate(Mode m, const QString& name, bool whole = false)
  {
    mode = m;
    appName = name;
    wholeApplication = whole;
  }

  ~QXNApplicationsModelPrivate() { };

  Mode mode;
  QString appName;
  bool wholeApplication;
};

#endif // QXNAPPLICATIONSMODELPRIVATE_H
