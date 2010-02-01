// Local
#include "QXNeur.h" // QXNeur class is subclassed QApplication needed to provide the X11 event handling

int main(int argc, char** argv)
{
  QXNeur app(argc, argv);
  return app.exec();
}
